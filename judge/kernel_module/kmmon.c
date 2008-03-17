#include <asm/errno.h>
#include <asm/types.h>
#include <asm/uaccess.h>

#include <linux/fcntl.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/mm.h>
#include <linux/module.h>
#include <linux/preempt.h>
#include <linux/rbtree.h>
#include <linux/sched.h>
#include <linux/signal.h>
#include <linux/slab.h>
#include <linux/string.h>

#include "syscall.h"
#include "kmmon.h"

struct {
  unsigned short limit;
  unsigned int base;
} __attribute__ ((packed)) idtr;

struct idt {
  unsigned short off_low;
  unsigned short seg_selector;
  unsigned char reserved;
  unsigned char flag;
  unsigned short off_high;
};

static struct idt *p_idt80;
static unsigned long origin_syscall;

static void** origin_syscall_table;

asmlinkage long (*old_ni_syscall)(void);
asmlinkage int (*old_clone)(struct pt_regs);
asmlinkage int (*old_fork)(struct pt_regs);
asmlinkage int (*old_vfork)(struct pt_regs);
asmlinkage unsigned long (*old_brk)(unsigned long);

asmlinkage void suicide(void) {
  send_sig(SIGKILL, current, 1);
}

asmlinkage int notify_tracer(int syscall) {
  struct task_struct* p = current;
  struct siginfo info;
  info.si_signo = KMMON_SIG;
  info.si_int = syscall;
  info.si_code = SI_QUEUE;
  info.si_pid = p->pid;
  info.si_uid = p->uid;
  while (true) {
    struct task_struct* q = p->parent;
    if (q == NULL)
      return 0;
    p = q;
    if (!(q->flags & KMMON_MASK))
      break;
  }
  if (p->pid == 1)
    return 0;

  current->exit_code = KMMON_SIG;
  rcu_read_lock();
  send_sig_info(KMMON_SIG, &info, p);
  rcu_read_unlock();
  while (current->exit_code == KMMON_SIG) {
    current->state = TASK_STOPPED;
    schedule();
    printk("wake up\n");
  }
  return current->exit_code;
}

extern void new_int80(void);

void asm_stuff(void) {
  __asm__ __volatile__ (
    ".globl new_int80\n"
    ".align 4, 0x90\n"
  "new_int80:\n"
    "pushl %%ebx;"
    "movl %%esp, %%ebx;"
    "andl %0, %%ebx;"
    "movl %c1(%%ebx), %%ebx;"
    "andl %2, %c3(%%ebx);"
    "jz normal;"
    "jnp disable;"
    "pushl %%ecx;"
    "pushl %%edx;"
    "pushl %%esi;"
    "pushl %%edi;"
    "pushl %%eax;"
    "call notify_tracer;"
    "andl $-1, %%eax;"
    "popl %%eax;"
    "popl %%edi;"
    "popl %%esi;"
    "popl %%edx;"
    "popl %%ecx;"
    "jz normal;"
  "disable:\n"
    "call suicide;"
    "movl $0xffff, %%eax;"
  "normal:\n"
    "popl %%ebx;"
    "jmp *%5;"
    :
    : "i"(-THREAD_SIZE),
      "i"(&((struct thread_info*)0)->task),
      "i"(KMMON_MASK),
      "i"(&((struct task_struct*)0)->flags),
      "i"(syscall_filter_table),
      "m"(origin_syscall)
  );
}

asmlinkage unsigned long kmmon(int request,
                               unsigned long pid,
                               unsigned long address,
                               unsigned long data) {
  struct task_struct* p;
  switch (request) {
    case KMMON_TRACEME :
      current->flags |= KMMON_MASK;
      break;
    case KMMON_CONTINUE :
    case KMMON_KILL :
    case KMMON_READMEM :
    case KMMON_GETREG :
      rcu_read_lock();
      p = find_task_by_pid(pid);
      if (!p || !(p->flags & KMMON_MASK) || !(p->state & TASK_STOPPED)) {
        printk(KERN_ERR "Invalid pid : %ld\n", pid);
        return -1;
      }
      if (request == KMMON_READMEM) {
        struct page* page;
        struct vm_area_struct* vma;
        int offset, len, tmp;
        if (p->mm == NULL) {
          printk(KERN_ERR "Fail to get user pages: %ld, %lx\n", pid, address);
          return -1;
        }
        if (get_user_pages(p, p->mm, address, 1, 0, 1, &page, &vma) <= 0) {
          printk(KERN_ERR "High mem page: %ld, %lx\n", pid, address);
          return -1;
        }
        offset = address & (PAGE_SIZE - 1);
        if (sizeof(data) > PAGE_SIZE - offset)
          len = PAGE_SIZE - offset;
        else
          len = sizeof(data);
        memcpy(&tmp, page_address(page) + offset, len);
        put_user(tmp, (unsigned long*)data);
      } else if (request == KMMON_GETREG) {
        put_user(*((int*)p->thread.esp0 - 6 - address), (unsigned long*)data);
      } else {
        p->exit_code = request == KMMON_KILL;
        wake_up_process(p);
      }
      rcu_read_unlock();
      break;
    default :
      return -1;
  }
  return 0;
}

#define DEFINE_CLONE(func) \
  asmlinkage int kmmon_ ## func (struct pt_regs regs) { \
    int ret = old_ ## func (regs); \
    if (!ret && (current->parent->flags & KMMON_MASK)) { \
      current->flags |= KMMON_MASK; \
    } \
    return ret; \
  }

DEFINE_CLONE(clone)

DEFINE_CLONE(fork)

DEFINE_CLONE(vfork)

asmlinkage unsigned long kmmon_brk(unsigned long brk) {
  unsigned long ret = old_brk(brk);
  if ((current->flags & KMMON_MASK) && brk && ret < brk) {
    notify_tracer(45);
    send_sig(SIGKILL, current, 1);
  }
  return ret;
}

int init(void) {
  char* p;
  unsigned new_syscall;
  origin_syscall_table = 0;
  __asm__ ("sidt %0":"=m"(idtr));
  p_idt80 = (struct idt*)(idtr.base + sizeof(struct idt) * 0x80);
  origin_syscall = (p_idt80->off_high << 16) | p_idt80->off_low;
  for (p = (char*)origin_syscall; p < (char*)origin_syscall + 1024; p++ ) {
    if (*(p + 0) == '\xff' && *(p + 1) == '\x14' && *(p + 2) == '\x85') {
      origin_syscall_table = (void**)*(unsigned long*)(p + 3);
      break;
    }
  }
  if (!origin_syscall_table) {
    printk(KERN_ERR "Fail to find syscall_table\n");
    return -1;
  }
  old_ni_syscall = origin_syscall_table[__NR_kmmon];
  old_clone = origin_syscall_table[__NR_clone];
  old_fork = origin_syscall_table[__NR_fork];
  old_vfork = origin_syscall_table[__NR_vfork];
  old_brk = origin_syscall_table[__NR_brk];
  origin_syscall_table[__NR_kmmon] = kmmon;
  origin_syscall_table[__NR_clone] = kmmon_clone;
  origin_syscall_table[__NR_fork] = kmmon_fork;
  origin_syscall_table[__NR_vfork] = kmmon_vfork;
  origin_syscall_table[__NR_brk] = kmmon_brk;
  new_syscall = (unsigned long)&new_int80;
  p_idt80->off_low = (unsigned short)(new_syscall & 0x0000ffff);
  p_idt80->off_high = (unsigned short)((new_syscall >> 16) & 0x0000ffff);
  return 0;
}

void cleanup(void) {
  origin_syscall_table[__NR_kmmon] = old_ni_syscall;
  origin_syscall_table[__NR_clone] = old_clone;
  origin_syscall_table[__NR_fork] = old_fork;
  origin_syscall_table[__NR_vfork] = old_vfork;
  origin_syscall_table[__NR_brk] = old_brk;
  p_idt80->off_low = (unsigned short)(origin_syscall & 0x0000ffff);
  p_idt80->off_high = (unsigned short)((origin_syscall >> 16) & 0x0000ffff);
  return;
}

module_init(init);
module_exit(cleanup);

MODULE_LICENSE("GPL v2");

