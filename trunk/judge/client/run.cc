// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <string>

#include <arpa/inet.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "base/flags.h"
#include "base/judge_result.h"
#include "base/logging.h"
#include "base/util.h"

#include "judge/client/result.h"
#include "judge/client/syscall.h"
#include "judge/client/trace.h"
#include "judge/client/utils.h"

#include "judge/client/run.h"

DECLARE_FLAGS(int, uid);
DECLARE_FLAGS(int, gid);

int monitor(int communicate_socket,
            pid_t pid,
            int time_limit,
            int memory_limit,
            TraceCallback* callback) {
  LOG(DEBUG) << "Start to monitor process " << pid;
  int result = -1;
  int time_ = 0;
  int memory_ = 0;
  int status;
  int ts;
  int ms;
  time_t last_time = time(NULL) - 1;
  time_t current_time = 0;

  while (result < 0 && !callback->hasExited()) {
    waitpid(pid, &status, 0);
    LOG(DEBUG) << "Get status from pid : " << stringPrintf("%04x", status);
    if (WIFEXITED(status))
      break;

    if ((status >> 8) != 0x05) {
      callback->processResult(status >> 8);
      result = callback->getResult();
      ptrace(PTRACE_KILL, pid, NULL, NULL);
      break;
    }

    int syscall = ptrace(PTRACE_PEEKUSER, pid, 4 * ORIG_EAX, NULL);
    LOG(DEBUG) << "Syscall " << syscall << " from " << pid;
    if (syscall_filter_table[syscall]) {
      callback->processSyscall(pid, syscall);
    } else {
      ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
    }

    current_time = time(NULL);
    if (current_time - last_time > 0) {
      last_time = current_time;

      ts = readTime(pid);
      ms = readMemory(pid);
      if (ts > time_)
        time_ = ts;
      if (ms > memory_)
        memory_ = ms;

      if (time_ > time_limit)
        result = TIME_LIMIT_EXCEEDED;
      if (memory_ > memory_limit)
        result = MEMORY_LIMIT_EXCEEDED;
    
      LOG(DEBUG) << "Monitor process " << pid << " with time/memory("
                 << time_ << "/" << memory_ << ")";
    }
  }

  if (callback->hasExited()) {
    waitpid(pid, &status, 0);
    LOG(DEBUG) << "Exited " << pid << " normally";
    result = 0;
  } else if (result < 0) {
    if (callback->getResult() == 0) {
      int ts = readTime(pid);
      int ms = readMemory(pid);
      if (ts > time_)
        time_ = ts;
      if (ms > memory_)
        memory_ = ms;
    }
    callback->processResult(status);
    result = callback->getResult();
    kill(pid, SIGKILL);
  }

  if (result == TIME_LIMIT_EXCEEDED)
    time_ = time_limit + 36;
  if (result == MEMORY_LIMIT_EXCEEDED)
    memory_ = memory_limit + 36;

  JudgeResult::getInstance()->updateTime(time_);
  JudgeResult::getInstance()->updateMemory(memory_);

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
  run_info.trace = true;
  TraceCallback callback;
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
    return SYSTEM_ERROR;
  }
  if (result) {
    sendReply(communicate_socket, result);
    return result;
  }
  return 0;
}

