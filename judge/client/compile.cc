// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <string>

#include "args.h"
#include "client.h"
#include "logging.h"
#include "util.h"

#include "compile.h"

int doCompile(int communicate_socket,
              const string& source_filename) {
  LOG(INFO) << "Compiling " << source_filename;
  sendReply(communicate_socket, COMPILING);
  string command = ARG_root + "/script/compile.sh " + source_filename;
  LOG(INFO) << "Compile command: " << command;

  int communicate_pipe[2];
  if (pipe(communicate_pipe) < 0) {
    LOG(SYS_ERROR) << "Fail to create communicate pipe "
                      "between judge and compile script";
    sendReply(communicate_socket, SYSTEM_ERROR);
    return -1;
  }
  RunInfo run_info;
  run_info.fd_stderr = communicate_pipe[1];
  run_info.time_limit = COMPILE_TIME_LIMIT;
  pid_t pid = create_shell_process(command.c_str(), run_info);
  close(fdPipe[1]);
  if (pid < 0) {
    LOG(SYS_ERROR) << "Fail to create shell process to compile";
    sendReply(communicate_socket, SYSTEM_ERROR);
  }
  return 0;
}

