// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <string>

#include <arpa/inet.h>
#include <errno.h>
#include <sys/wait.h>

#include "base/flags.h"
#include "base/logging.h"
#include "base/util.h"
#include "base/judge_result.h"

#include "judge/client/client.h"
#include "judge/client/trace.h"
#include "judge/client/utils.h"

#include "judge/client/compile.h"

DECLARE_FLAGS(string, root_dir);

int doCompile(int communicate_socket,
              const string& source_filename) {
  LOG(INFO) << "Compiling " << source_filename;
  sendReply(communicate_socket, COMPILING);
  string command = FLAGS_root_dir + "/bin/compile.sh " + source_filename;
  LOG(INFO) << "Compile command: " << command;

  int file_pipe[2];
  if (pipe(file_pipe) < 0) {
    LOG(ERROR) << "Fail to create communicate pipe "
                  "between judge and compile script";
    sendReply(communicate_socket, SYSTEM_ERROR);
    return -1;
  }
  RunInfo run_info;
  run_info.file_stderr = file_pipe[1];
  run_info.time_limit = COMPILE_TIME_LIMIT;

  class Callback : public TraceCallback {
    public :
      virtual void onSigchld(pid_t) {}
  } callback;

  pid_t pid = createShellProcess(command.c_str(), run_info);
  close(file_pipe[1]);
  if (pid < 0) {
    LOG(ERROR) << "Fail to create shell process to compile";
    close(file_pipe[0]);
    sendReply(communicate_socket, SYSTEM_ERROR);
    return -1;
  }
  LOG(DEBUG) << "Create process " << pid << " to compile";

  static signed char error_message[16384];
  int message_length = socket_read(file_pipe[0],
                                   error_message,
                                   sizeof(error_message));
  close(file_pipe[0]);
  if (message_length < 0) {
    LOG(ERROR) << "Fail to read compile error message";
    sendReply(communicate_socket, SYSTEM_ERROR);
    return -1;
  }

  int status;
  while (waitpid(pid, &status, 0) < 0) {
    if (errno != EINTR) {
      LOG(SYS_ERROR) << "System error in compiling process";
      sendReply(communicate_socket, SYSTEM_ERROR);
      return -1;
    }
  }
  if (WIFSIGNALED(status)) {
    LOG(ERROR) << "Compilation terminated by signal " << WTERMSIG(status);
    sendReply(communicate_socket, SYSTEM_ERROR);
    return -1;
  }
  status = WEXITSTATUS(status);
  if (status >= 126) {
    LOG(INFO) << "Compilation failed";
    sendReply(communicate_socket, SYSTEM_ERROR);
    return -1;
  } else if (status) {
    LOG(INFO) << "Compilation error : " << endl
              << error_message;
    sendReply(communicate_socket, COMPILE_ERROR);

    // Send CE message back to server
    uint16_t length = htons(message_length);
    socket_write(communicate_socket, &length, sizeof(length));
    LOG(INFO) << "Send CE message length finished. length = " << message_length;
    for (int i = 0; i < message_length; ++i) {
      if (error_message[i] <= 0)
        error_message[i] = '?';
    }
    socket_write(communicate_socket, error_message, message_length);
    LOG(INFO) << "Send CE message finished.";
    return -1;
  }
  LOG(INFO) << "Compilation done";
  return 0;
}

