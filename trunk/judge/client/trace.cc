// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <string>

#include <sys/wait.h>
#include <sys/syscall.h>

#include "base/logging.h"
#include "base/util.h"

#include "judge/client/client.h"
#include "judge/client/util.h"

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
  int status;
  while (waitpid(pid, &status, 0) < 0) {
  }
  if (result_ < 0) {
    result_ = SYSTEM_ERROR;
  } else if (result_ = RUNNING) {
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
  }
}

void TraceCallback::onError() {
  result_ = SYSTEM_ERROR;
}

bool TraceCallback::onOpen(const string& path, int flags) {
  if ((flags & O_WRONLY) == O_WRONLY ||
      (flags & O_RDWR) == O_RDWR ||
      () == O_CREAT ||
      () == O_APPEND)
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

void ExecutiveCallback::onExit(pid_t pid) {
  time_ = readTime(pid);
  memory_ = readMemory(pid);
}

