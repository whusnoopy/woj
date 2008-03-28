// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <string>

#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/syscall.h>

#include "base/logging.h"
#include "base/util.h"
#include "base/judge_result.h"

#include "judge/client/utils.h"
#include "judge/kernel_module/kmmon-lib.h"

#include "judge/client/trace.h"

TraceCallback* TraceCallback::instance_;

bool TraceCallback::onExecve() {
  if (result_ < 0) {
    result_ = RUNNING;
    return true;
  } else {
    return false;
  }
}

void TraceCallback::onMemoryLimitExceeded() {
  result_ = MEMORY_LIMIT_EXCEEDED;
}

void TraceCallback::onExit(pid_t pid) {
  time_ = readTime(pid);
  memory_ = readMemory(pid);
  result_ = 0;
}

void TraceCallback::onSigchld(pid_t pid) {
  exited_ = true;
}

void TraceCallback::onError() {
  result_ = SYSTEM_ERROR;
}

bool TraceCallback::onOpen(const string& path, int flags) {
  if ((flags & O_WRONLY) == O_WRONLY ||
      (flags & O_RDWR) == O_RDWR ||
      (flags & O_CREAT) == O_CREAT ||
      (flags & O_APPEND) == O_APPEND)
    return false;
  if (path.empty())
    return false;
  if (path[0] == '/' || path[0] == '.') {
    if (path.find("/proc/") == 0 ||
        path.rfind(".so") == path.size() - 3 ||
        path.rfind(".a") == path.size() - 2)
      return false;
  }
  return true;
}

void TraceCallback::processResult(int status) {
  switch (result_) {
    case -1 :
      result_ = SYSTEM_ERROR;
      break;
    case RUNNING :
      switch (WTERMSIG(status)) {
        case SIGXCPU :
          LOG(INFO) << "Time Limit Exceeded";
          result_ = TIME_LIMIT_EXCEEDED;
          break;
        case SIGXFSZ :
          LOG(INFO) << "Output Limit Exceeded";
          result_ = OUTPUT_LIMIT_EXCEEDED;
          break;
        case SIGSEGV :
          LOG(INFO) << "Runtime Error - SIGSEGV";
          result_ = RUNTIME_ERROR_SIGSEGV;
          break;
        case SIGFPE :
          LOG(INFO) << "Runtime Error - SIGFPE";
          result_ = RUNTIME_ERROR_SIGFPE;
          break;
        case SIGBUS :
          LOG(INFO) << "Runtime Error - SIGBUS";
          result_ = RUNTIME_ERROR_SIGBUS;
          break;
        case SIGKILL :
        case SIGABRT :
          LOG(INFO) << "Runtime Error - SIGABRT";
          result_ = RUNTIME_ERROR_SIGABRT;
          break;
        default :
          LOG(ERROR) << "Unexpected signal : " << WTERMSIG(status);
          result_ = SYSTEM_ERROR;
      }
      break;
    case MEMORY_LIMIT_EXCEEDED :
      LOG(INFO) << "Memory limit exceeded";
      break;
  }
}

static void sigchldHandler(int sig, siginfo_t* siginfo, void* context) {
  if (TraceCallback::getInstance()) {
    TraceCallback::getInstance()->onSigchld(siginfo->si_pid);
  }
}

static int readStringFromTracedProcess(pid_t pid,
                                       int address,
                                        char* buffer,
                                        int maxLength) {
  for (int i = 0; i < maxLength; i += 4) {
    int data;
    if (kmmon_readmem(pid, address + i, &data) < 0)
      return -1;
    char* cs = reinterpret_cast<char*>(&data);
    for (int j = 0; j < 4; j++, cs++) {
      if (*cs && i + j < maxLength) {
        buffer[i + j] = *cs;
      } else {
        buffer[i + j] = 0;
        return 0;
      }
    }
  }
  buffer[maxLength - 1] = 0;
  return 0;
}

static void sigkmmonHandler(int sig, siginfo_t* siginfo, void* context) {
  LOG(DEBUG) << "sigkmmonHandler catched " << siginfo->si_pid;
  TraceCallback* callback = TraceCallback::getInstance();
  pid_t pid = siginfo->si_pid;
  if (!callback) {
    LOG(INFO) << "No callback instance found";
    kmmon_continue(pid);
    return;
  }
  
  int syscall = siginfo->si_int;
  switch (syscall) {
    case SYS_exit :
    case SYS_exit_group :
      callback->onExit(pid);
      kmmon_continue(pid);
      break;

    case SYS_brk :
      callback->onMemoryLimitExceeded();
      kmmon_kill(pid);
      break;

    case SYS_clone :
    case SYS_fork :
    case SYS_vfork :
      if (callback->onClone())
        kmmon_continue(pid);
      else
        kmmon_kill(pid);
      break;

    case SYS_execve :
      if (callback->onExecve())
        kmmon_continue(pid);
      else
        kmmon_kill(pid);
      break;

    case SYS_open :
      char buffer[PATH_MAX + 1];
      int address, flags;
      if (kmmon_getreg(pid, KMMON_REG_EBX, &address) < 0 ||
          kmmon_getreg(pid, KMMON_REG_ECX, &flags) < 0) {
        LOG(ERROR) << "Fail to read register values and flags from "
                   << "traced process";
        callback->onError();
      } else if (readStringFromTracedProcess(pid,
                                             address,
                                             buffer,
                                             sizeof(buffer)) < 0) {
        LOG(ERROR) << "Fail to read memory from traced process";
        callback->onError();
        kmmon_kill(pid);
      } else if (callback->onOpen(buffer, flags)) {
        kmmon_continue(pid);
      } else {
        kmmon_kill(pid);
      }
      break;
    default :
      LOG(ERROR) << "Unexpected syscall : " << syscall;
      TraceCallback::getInstance()->onError();
      kmmon_kill(pid);
  }
}

static struct sigaction sigchld_act;

void installHandlers() {
  struct sigaction new_action;
  struct sigaction oact;
  new_action.sa_sigaction = sigkmmonHandler;
  sigemptyset(&new_action.sa_mask);
  new_action.sa_flags = SA_SIGINFO;
  sigaction(KMMON_SIG, &new_action, NULL);

  sigaction(KMMON_SIG, NULL, &oact);
  if (oact.sa_sigaction != sigkmmonHandler) {
    LOG(SYS_ERROR) << "Can't modify KMMON_SIG to sigkmmonHandler";
    raise(SIGKILL);
  } else {
    LOG(DEBUG) << "Modity KMMON_SIG to sigkmmonHandler";
  }

  new_action.sa_sigaction = sigchldHandler;
  sigaction(SIGCHLD, &new_action, &sigchld_act);
  
  sigaction(SIGCHLD, NULL, &oact);
  if (oact.sa_sigaction != sigchldHandler) {
    LOG(SYS_ERROR) << "Can't modify SIGCHLD to sigchldHandler";
    raise(SIGKILL);
  } else {
    LOG(DEBUG) << "Modify SIGCHLD to sigchldHandler";
  }
}

void uninstallHandlers() {
  sigaction(SIGCHLD, &sigchld_act, NULL);
}

