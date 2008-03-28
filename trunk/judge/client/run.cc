// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <string>

#include <arpa/inet.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "base/flags.h"
#include "base/judge_result.h"
#include "base/logging.h"
#include "base/util.h"

#include "judge/client/trace.h"
#include "judge/client/utils.h"

#include "judge/client/run.h"

DECLARE_FLAGS(int, uid);
DECLARE_FLAGS(int, gid);

int sendRunningMessage(int communicate_socket, int time_, int memory_) {
  static char message[9];
  message[0] = RUNNING;
  *(unsigned int*)(message + 1) = htonl((unsigned int)(time_));
  *(unsigned int*)(message + 5) = htonl((unsigned int)(memory_));
  if (socket_write(communicate_socket, message, sizeof(message)) == -1) {
    LOG(ERROR) << "Fail to send running message back";
    return -1;
  }
  return 0;
}

int monitor(int communicate_socket,
            pid_t pid,
            int time_limit,
            int memory_limit,
            TraceCallback* callback) {
  LOG(INFO) << "Start to monitor process " << pid;
  int result = -1;
  int time_ = 0;
  int memory_ = 0;
  while (result < 0 && !callback->hasExited()) {
    struct timespec request, remain;
    request.tv_sec = 1;
    request.tv_nsec = 0;
    while (result < 0 &&
           !callback->hasExited() &&
           nanosleep(&request, &remain) < 0) {
      if (errno != EINTR) {
        LOG(SYS_ERROR) << "Fail to nanosleep";
        kill(pid, SIGKILL);
        result = SYSTEM_ERROR;
      }
      request = remain;
    }
    int ts;
    int ms;
    if (result < 0 && !callback->hasExited()) {
      ts = callback->getTime();
      ms = callback->getMemory();
      if (ts > time_)
        time_ = ts;
      if (ms > memory_)
        memory_ = ms;
      if (time_ > time_limit) {
        result = TIME_LIMIT_EXCEEDED;
        time_ = time_limit + 36;
      }
      if (memory_ > memory_limit) {
        result = MEMORY_LIMIT_EXCEEDED;
        memory_ = memory_limit + 36;
      }
      
      LOG(INFO) << "Monitor process " << pid << " with time/memory("
                << time_ << "/" << memory_ << ")";

      if (sendRunningMessage(communicate_socket, time_, memory_)) {
        if (!callback->hasExited())
          kill(pid, SIGKILL);
        result = SYSTEM_ERROR;
        break;
      }
    }
  }

  int status;
  while (waitpid(pid, &status, 0) < 0) {
    if (errno != EINTR) {
      LOG(SYS_ERROR) << "Error when waitpid for " << pid << " with errno = "
                     << errno;
      return SYSTEM_ERROR;
    }
  }
  if (!WIFEXITED(status)) {
    LOG(SYS_ERROR) << "Child process not exited";
    return SYSTEM_ERROR;
  }

  if (result < 0) {
    if (callback->getResult() == 0) {
      time_ = callback->getTime();
      memory_ = callback->getMemory();
    }
    callback->processResult(status);
    result = callback->getResult();
    if (result == TIME_LIMIT_EXCEEDED)
      time_ = time_limit + 36;
    if (result == MEMORY_LIMIT_EXCEEDED)
      memory_ = memory_limit + 36;
    if (sendRunningMessage(communicate_socket, time_, memory_))
      return SYSTEM_ERROR;
  }
  return result;
}

int runExe(int communicate_socket,
           const string& exe_filename,
           const string& input_filename,
           const string& program_output_filename,
           int time_limit,
           int memory_limit,
           int output_limit) {
  LOG(INFO) << "Running";
  const char* commands[] = {exe_filename.c_str(), exe_filename.c_str(), NULL};
  RunInfo run_info;
  run_info.stdin_filename = input_filename.c_str();
  run_info.stdout_filename = program_output_filename.c_str();
  run_info.uid = FLAGS_uid;
  run_info.gid = FLAGS_gid;
  run_info.time_limit = time_limit;
  run_info.memory_limit = memory_limit;
  run_info.output_limit = output_limit;
  run_info.proc_limit = 1;
  run_info.file_limit = 5;
  run_info.trace = 1;
  ExecutiveCallback callback;
  pid_t pid = createProcess(commands, run_info);
  if (pid == -1) {
    LOG(ERROR) << "Fail to execute program : " << exe_filename;
    return SYSTEM_ERROR;
  }
  return monitor(communicate_socket, pid, time_limit, memory_limit, &callback);
}

inline int isNativeExe(const string& source_file_type) {
  return source_file_type == "cc" ||
         source_file_type == "c" ||
         source_file_type == "pas";
}

int doRun(int communicate_socket,
          const string& program_name,
          const string& source_file_type,
          const string& input_filename,
          const string& program_output_filename,
          int time_limit,
          int memory_limit,
          int output_limit) {
  int result = 0;
  if (isNativeExe(source_file_type)) {
    LOG(INFO) << "Executive program, run \"" << program_name << "\"";
    result = runExe(communicate_socket,
                    program_name,
                    input_filename,
                    program_output_filename,
                    time_limit,
                    memory_limit,
                    output_limit);
  } else {
    return -1;
  }
  if (result) {
    sendReply(communicate_socket, result);
    if (result == SYSTEM_ERROR)
      return -1;
    else
      return 1;
  }
  return 0;
}

