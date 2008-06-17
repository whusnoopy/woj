// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <string>

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#include "base/flags.h"
#include "base/judge_result.h"
#include "base/logging.h"
#include "base/util.h"

#include "judge/client/client.h"
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
/*
  static int clktck = 0;
  if (clktck == 0) {
    clktck = sysconf(_SC_CLK_TCK);
  }
      
  clock_t last_clock = clock() - clktck;
  clock_t current_clock = 0;
*/
  while (result < 0 && !callback->hasExited()) {
    waitpid(pid, &status, 0);
//    LOG(DEBUG) << "Get status from pid : " << stringPrintf("%04x", status);
    if (WIFEXITED(status))
      break;

    if ((status >> 8) != 0x05) {
      callback->processResult(status >> 8);
      result = callback->getResult();
      ptrace(PTRACE_KILL, pid, NULL, NULL);
      break;
    }

    int syscall = ptrace(PTRACE_PEEKUSER, pid, 4 * ORIG_EAX, NULL);
    if (syscall_filter_table[syscall]) {
      callback->processSyscall(pid, syscall);
      /*
      if (syscall == SYS_brk) {
        ms = readMemory(pid);
        if (ms > memory_)
          memory_ = ms;
        if (memory_ > memory_limit)
          result = MEMORY_LIMIT_EXCEEDED;
      }
      */
    } else {
      ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
    }
/*
    current_clock = clock();
    LOG(DEBUG) << current_clock << " equal to " 
               << current_clock * 1.0 / clktck << "s";
    if ((current_clock - last_clock) * 100 / clktck > 0) {
      last_clock = current_clock;

//      ts = readTime(pid);
      ms = readMemory(pid);
//      if (ts > time_)
//        time_ = ts;
      if (ms > memory_)
        memory_ = ms;

//      if (time_ > time_limit)
//        result = TIME_LIMIT_EXCEEDED;
      if (memory_ > memory_limit)
        result = MEMORY_LIMIT_EXCEEDED;
    
      LOG(DEBUG) << "Monitor process " << pid << " with time/memory("
                 << time_ << "/" << memory_ << ")";
    }
*/
  }

  if (callback->hasExited()) {
    struct rusage current_rusage;
    wait4(pid, &status, 0, &current_rusage);

    ts = current_rusage.ru_utime.tv_sec * 1000 +
         current_rusage.ru_utime.tv_usec / 1000;
    if (ts > time_)
      time_ = ts;

    ms = current_rusage.ru_minflt * 4;
    if (ms > memory_)
      memory_ = ms;

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
  sendReply(communicate_socket, RUNNING);
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

int runJava(int communicate_socket,
            const string& class_directory,
            const string& input_filename,
            const string& program_output_filename,
            const string& java_error_output_filename,
            int time_limit,
            int memory_limit,
            int output_limit) {
  LOG(INFO) << "Running";
  sendReply(communicate_socket, RUNNING);
  const char* commands[] = {"/usr/bin/java",
                            "/usr/bin/java",
                            "-cp",
                            class_directory.c_str(),
                            "-Djava.security.manager",
                            "Main",
                            NULL};
  RunInfo run_info;
  run_info.stdin_filename = input_filename.c_str();
  run_info.stdout_filename = program_output_filename.c_str();
  run_info.stderr_filename = java_error_output_filename.c_str();
  run_info.uid = FLAGS_uid;
  run_info.gid = FLAGS_gid;
  run_info.time_limit = time_limit * JAVA_TIME_LIMIT_MULTIPLE +
                        JAVA_TIME_LIMIT_REMAINDER;
  run_info.memory_limit = memory_limit;
  run_info.output_limit = output_limit;
  run_info.proc_limit = 1;
  run_info.file_limit = 5;
  run_info.trace = false;
  TraceCallback callback;
  pid_t pid = createProcess(commands, run_info);
  if (pid == -1) {
    LOG(ERROR) << "Fail to run java Main.class in " << class_directory;
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
          const string& working_dir,
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
  } else if (source_file_type == "java") {
    LOG(INFO) << "Run java program during javac \"" << program_name << "\"";
    string java_error_output_filename = "/tmp/java_error";
    result = runJava(communicate_socket,
                     working_dir,
                     input_filename,
                     program_output_filename,
                     java_error_output_filename,
                     time_limit,
                     memory_limit,
                     output_limit);

    // Get error message length and the message text at most 16384 Bytes
    static signed char error_message[16384];
    int error_file = open(java_error_output_filename.c_str(), O_RDWR, 0777);
    int message_length = socket_read(error_file,
                                     error_message,
                                     sizeof(error_message));

    // If there is error info output, we see RUNTIME_ERROR_JAVA
    if (message_length > 0) {
      result = RUNTIME_ERROR_JAVA;
      sendReply(communicate_socket, result);

      // Send RE_JAVA message back to server
      uint16_t length = htons(message_length);
      socket_write(communicate_socket, &length, sizeof(length));
      LOG(INFO) << "Send RE_JAVA message length finished. length = " << message_length;
      for (int i = 0; i < message_length; ++i) {
        if (error_message[i] <= 0)
          error_message[i] = '?';
      }
      socket_write(communicate_socket, error_message, message_length);
      close(error_file);
      LOG(INFO) << "Send RE_JAVA message finished.";
      return result;
    }
  } else {
    return SYSTEM_ERROR;
  }

  if (result) {
    sendReply(communicate_socket, result);
    LOG(INFO) << "Program terminated because the result " << result;
    return result;
  }
  return 0;
}

