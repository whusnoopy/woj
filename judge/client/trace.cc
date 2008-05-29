// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <string>

#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <sys/ptrace.h>
#include <sys/reg.h>

#include "base/logging.h"
#include "base/util.h"
#include "base/judge_result.h"

#include "judge/client/utils.h"

#include "judge/client/trace.h"

TraceCallback* TraceCallback::instance_;

bool TraceCallback::onExecve() {
  LOG(DEBUG) << "Start to execve";
  if (result_ < 0) {
    result_ = RUNNING;
    return true;
  } else {
    return false;
  }
}

void TraceCallback::onExit() {
  LOG(DEBUG) << "On Exit Status";
  exited_ = true;
}

void TraceCallback::onMemoryLimitExceeded() {
  LOG(DEBUG) << "On Memory Limit Exceeded Status";
  result_ = MEMORY_LIMIT_EXCEEDED;
}

void TraceCallback::onError() {
  LOG(DEBUG) << "On Unknown Error Status";
  result_ = SYSTEM_ERROR;
}

void TraceCallback::onRestrictedFunction() {
  LOG(INFO) << "Restricted Function";
  result_ = RESTRICTED_FUNCTION;
}

bool TraceCallback::onOpen(const string& path, int flags) {
  LOG(DEBUG) << "Test file \'" << path << "\' with flags = " << flags;
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
        case SIGALRM :
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

static int readStringFromTracedProcess(pid_t pid,
                                       int address,
                                       char* buffer,
                                       int maxLength) {
  for (int i = 0; i < maxLength; i += 4) {
    int data;
    data = ptrace(PTRACE_PEEKDATA, pid, address + i, NULL);
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

void TraceCallback::processSyscall(pid_t pid, int syscall) {
  LOG(DEBUG) << "Catched Syscall " << syscall << " from " << pid;

  TraceCallback* callback = TraceCallback::getInstance();
  if (!callback) {
    LOG(INFO) << "No callback instance found";
    ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
    return;
  }
  
  switch (syscall) {
    case SYS_exit :
    case SYS_exit_group :
      callback->onExit();
      ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
      break;

    case SYS_brk :
      unsigned long brk;
      brk = ptrace(PTRACE_PEEKUSER, pid, 4 * EBX, NULL);
      if (brk) {
        callback->onMemoryLimitExceeded();
        ptrace(PTRACE_KILL, pid, NULL, NULL);
      } else {
        ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
      }
      break;

    case SYS_clone :
    case SYS_fork :
    case SYS_vfork :
      if (callback->onClone()) {
        ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
      } else {
        callback->onRestrictedFunction();
        ptrace(PTRACE_KILL, pid, NULL, NULL);
      }
      break;

    case SYS_execve :
      if (callback->onExecve())
        ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
      else
        ptrace(PTRACE_KILL, pid, NULL, NULL);
      break;

    case SYS_open :
    case SYS_access :
      char buffer[PATH_MAX + 1];
      unsigned long address;
      address = ptrace(PTRACE_PEEKUSER, pid, 4 * EBX, NULL);
      unsigned long flags;
      flags = ptrace(PTRACE_PEEKUSER, pid, 4 * ECX, NULL);
      LOG(DEBUG) << "read EBX as address = " << address;
      LOG(DEBUG) << "read ECX as flags = " << flags;
      if (address < 0 || flags < 0) {
        LOG(ERROR) << "Fail to read register values and flags from "
                   << "traced process";
        callback->onError();
      } else if (readStringFromTracedProcess(pid,
                                             address,
                                             buffer,
                                             sizeof(buffer)) < 0) {
        LOG(ERROR) << "Fail to read memory from traced process";
        callback->onError();
        ptrace(PTRACE_KILL, pid, NULL, NULL);
      } else if (callback->onOpen(buffer, flags)) {
        ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
      } else {
        callback->onRestrictedFunction();
        ptrace(PTRACE_KILL, pid, NULL, NULL);
      }
      break;
    default :
      LOG(ERROR) << "Unexpected syscall : " << syscall;
      TraceCallback::getInstance()->onRestrictedFunction();
      ptrace(PTRACE_KILL, pid, NULL, NULL);
  }
}

