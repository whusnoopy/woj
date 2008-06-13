// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <string>
#include <stdlib.h>

#include <errno.h>
#include <sys/wait.h>
#include <sys/ptrace.h>
#include <sys/reg.h>

#include "base/flags.h"
#include "base/judge_result.h"
#include "base/logging.h"
#include "base/util.h"

#include "judge/client/client.h"
#include "judge/client/syscall.h"
#include "judge/client/trace.h"
#include "judge/client/utils.h"

#include "judge/client/judge.h"

DECLARE_FLAGS(int, uid);
DECLARE_FLAGS(int, gid);

static int compareFiles(const string& standard_output_file_name,
                        const string& users_output_file_name) {
  /*
  // Output files compare result to debug log
  FILE* file1 = fopen(standard_output_file_name.c_str(), "r");
  FILE* file2 = fopen(users_output_file_name.c_str(), "r");
  if (file1 == 0) {
    LOG(ERROR) << "Fail to open file " << standard_output_file_name;
    return SYSTEM_ERROR;
  }
  if (file2 == 0) {
    LOG(ERROR) << "Fail to open file " << users_output_file_name;
    return SYSTEM_ERROR;
  }
  
  char buf1[1024];
  char buf2[1024];
  memset(buf1, 0, sizeof(buf1));
  memset(buf2, 0, sizeof(buf2));
  
  while (!feof(file1) && !feof(file2)) {
    fgets(buf1, 1023, file1);
    LOG(DEBUG) << "# " << buf1;

    fgets(buf2, 1023, file2);
    LOG(DEBUG) << "* " << buf2;

    if (strcmp(buf1, buf2))
      break;
  }
  // Debug finished
  */

  FILE* standard_output_file = fopen(standard_output_file_name.c_str(), "r");
  FILE* users_output_file = fopen(users_output_file_name.c_str(), "r");
  if (standard_output_file == 0) {
    LOG(ERROR) << "Fail to open file " << standard_output_file_name;
    return SYSTEM_ERROR;
  }
  if (users_output_file == 0) {
    LOG(ERROR) << "Fail to open file " << users_output_file_name;
    return SYSTEM_ERROR;
  }

  int result = ACCEPTED;

  int c1 = fgetc(standard_output_file);
  int c2 = fgetc(users_output_file);

  while (c1 > 0 && c2 > 0) {
    if (c1 == c2) {
      c1 = fgetc(standard_output_file);
      c2 = fgetc(users_output_file);
    } else if (isspace(c1) || isspace(c2)) {
      while (isspace(c1)) {
        c1 = fgetc(standard_output_file);
      }
      while (isspace(c2)) {
        c2 = fgetc(users_output_file);
      }
      result = PRESENTATION_ERROR;
    } else {
      return WRONG_ANSWER;
    }
  }
  LOG(DEBUG) << "After touch a file end, the result is " << result;

  while (isspace(c1))
    c1 = fgetc(standard_output_file);
  while (isspace(c2))
    c2 = fgetc(users_output_file);

  if (c1 > 0 || c2 > 0)
    return WRONG_ANSWER;

  return result;
}

static int runSpecialJudge(const string& special_judge_exe_filename,
                           const string& standard_input_filename,
                           const string& users_output_filename) {
  string working_directory = special_judge_exe_filename.substr(0,
                                 special_judge_exe_filename.rfind('/') + 1);
  string special_judge_filename = special_judge_exe_filename.substr(
                                      working_directory.size());
  char path[MAX_PATH_LENGTH + 1];
  getcwd(path, sizeof(path));
  const char* commands[] = {
    special_judge_exe_filename.c_str(),
    special_judge_exe_filename.c_str(),
    standard_input_filename.c_str(),
    users_output_filename.c_str(),
    NULL
  };

  RunInfo run_info;
  run_info.uid = FLAGS_uid;
  run_info.gid = FLAGS_gid;
  run_info.time_limit = SPECIAL_JUDGE_TIME_LIMIT;
  run_info.memory_limit = 256 * 1024;
  run_info.output_limit = 16;
  run_info.file_limit = 6;
  run_info.trace = true;
  TraceCallback callback;

  pid_t pid = createProcess(commands, run_info);
  if (pid == -1) {
    LOG(ERROR) << "Fail to execute special judge";
    return SYSTEM_ERROR;
  } else {
    LOG(DEBUG) << "Create process " << pid << " to run special judge";
  }

  int status;
  int syscall;
  while (!callback.hasExited()) {
    waitpid(pid, &status, 0);
    if (WIFEXITED(status))
      break;
    
    syscall = ptrace(PTRACE_PEEKUSER, pid, 4 * ORIG_EAX, NULL);
    LOG(DEBUG) << "Syscall " << syscall << " from " << pid;
    if (syscall_filter_table[syscall]) {
      callback.processSyscall(pid, syscall);
    } else {
      ptrace(PTRACE_SYSCALL, pid, NULL, NULL);
    }
  }
  
  if (!callback.hasExited()) {
    callback.processResult(status);
    LOG(SYS_ERROR) << "SPJ Errored with result = " << callback.getResult();
    return SYSTEM_ERROR;
  } else {
    waitpid(pid, &status, 0);
    switch (WEXITSTATUS(status)) {
      case 0 :
        return ACCEPTED;
      case 2 :
        return PRESENTATION_ERROR;
      default :
        return WRONG_ANSWER;
    }
  }
}

int doJudge(int communicate_socket,
            const string& standard_input_file_name,
            const string& standard_output_file_name,
            const string& users_output_file_name,
            const string& special_judge_executable_file_name) {
  sendReply(communicate_socket, JUDGING);
  int result;
  if (!special_judge_executable_file_name.empty()) {
    LOG(INFO) << "Run special judge for " << users_output_file_name;
    result = runSpecialJudge(special_judge_executable_file_name,
                             standard_input_file_name,
                             users_output_file_name);
  } else {
    LOG(INFO) << "Judge between " << standard_output_file_name << " and "
              << users_output_file_name;
    result = compareFiles(standard_output_file_name,
                          users_output_file_name);
  }
  sendReply(communicate_socket, result);
  switch(result) {
    case ACCEPTED : 
      LOG(INFO) << "Accepted";
      break;
    case PRESENTATION_ERROR :
      LOG(INFO) << "Presentation Error";
      break;
    case WRONG_ANSWER :
      LOG(INFO) << "Wrong Answer";
      break;
  }
  return result;
}

