// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <string>
#include <stdlib.h>

#include <errno.h>
#include <sys/wait.h>

#include "base/flags.h"
#include "base/judge_result.h"
#include "base/logging.h"
#include "base/util.h"

#include "judge/client/trace.h"
#include "judge/client/utils.h"

#include "judge/client/judge.h"

DECLARE_FLAGS(int, uid);
DECLARE_FLAGS(int, gid);

static int compareFiles(const string& standard_output_file_name,
                        const string& users_output_file_name) {
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

  if (isspace(c1)) {
    while (isspace(c1))
      c1 = fgetc(standard_output_file);
    result = PRESENTATION_ERROR;
  }
  if (isspace(c2)) {
    while (isspace(c2))
      c2 = fgetc(users_output_file);
    result = PRESENTATION_ERROR;
  }

  if (c1 > 0 || c2 > 0)
    return WRONG_ANSWER;

  return result;
}

// TODO: Finish this function
static int runSpecialJudge(const string& special_judge_executable_filename,
                           const string& standard_input_filename,
                           const string& users_output_filename) {
  string working_directory =
    users_output_filename.substr(0, users_output_filename.rfind('/') + 1);
  const char* commands[] = {
    special_judge_executable_filename.c_str(),
    special_judge_executable_filename.c_str(),
    standard_input_filename.c_str(),
    users_output_filename.c_str(),
    NULL
  };
  RunInfo run_info;
  run_info.uid = FLAGS_uid;
  run_info.gid = FLAGS_gid;
  run_info.time_limit = 10;
  run_info.memory_limit = 256 * 1024;
  run_info.output_limit = 16;
  run_info.file_limit = 6;
  run_info.trace = true;
  run_info.working_dir = working_directory.c_str();
  ExecutiveCallback callback;

  pid_t pid = createProcess(commands, run_info);
  if (pid == -1) {
    LOG(ERROR) << "Fail to execute special judge";
    return SYSTEM_ERROR;
  }
  int status;
  while (waitpid(pid, &status, 0) < 0) {
    if (errno != EINTR) {
      LOG(SYS_ERROR) << "System error when waiting for special judge status";
      return SYSTEM_ERROR;
    }
  }
  if (WIFSIGNALED(status)) {
    LOG(ERROR) << "Special judge terminated by signal " << WTERMSIG(status);
    return SYSTEM_ERROR;
  }
  switch (WEXITSTATUS(status)) {
    case 0 :
      return ACCEPTED;
    case 2 :
      return PRESENTATION_ERROR;
    default :
      return WRONG_ANSWER;
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

