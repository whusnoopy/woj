#ifndef _JUDGE_KERNEL_MODULE_KMMON_LIB_H__
#define _JUDGE_KERNEL_MODULE_KMMON_LIB_H__

#include <sys/types.h>

#include "kmmon.h"

#ifdef __cplusplus
extern "C" {
#endif

int kmmon_traceme(void);

int kmmon_continue(pid_t pid);

int kmmon_kill(pid_t pid);

int kmmon_getreg(pid_t pid, int regno, int* value);

int kmmon_readmem(pid_t pid, unsigned long addr, int* value);

#ifdef __cplusplus
}
#endif

#define KMMON_REG_EAX 0
#define KMMON_REG_EBX 1
#define KMMON_REG_ECX 2
#define KMMON_REG_EDX 2
#define KMMON_REG_ESI 3
#define KMMON_REG_EDI 4
#define KMMON_REG_EBP 5

#endif

