// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>

#include <arpa/inet.h>
#include <errno.h>
#include <sys/wait.h>

#include "base/flags.h"
#include "base/logging.h"
#include "base/judge_result.h"
#include "base/util.h"

#include "judge/client/compile.h"
#include "judge/client/judge.h"
#include "judge/client/run.h"
#include "judge/client/trace.h"
#include "judge/client/utils.h"

using namespace std;

DECLARE_FLAGS(string, root_dir);

DEFINE_FLAGS(int, uid, "The uid for judge run program and special judge");
DEFINE_FLAGS(int, gid, "The gid for judge run program and special judge");

int getHeader(int communicate_socket,
              string& source_suffix,
              int& problem_id,
              int& version) {
  unsigned char header[9];
  int num = socket_read(communicate_socket, header, sizeof(header));
  if (num < sizeof(header))
    return -1;
  if (header[0] > sizeof(SOURCE_FILE_SUFFIX) / sizeof(SOURCE_FILE_SUFFIX)) {
    LOG(ERROR) << "Invalid source file type";
    sendReply(communicate_socket, SYSTEM_ERROR);
    return -1;
  }
  source_suffix = SOURCE_FILE_SUFFIX[header[0]];
  if (!isSupportedSourceFileType(source_suffix)) {
    LOG(ERROR) << "Unsupported source file type";
    sendReply(communicate_socket, UNSUPPORTED_SOURCE_FILE_TYPE);
    return -1;
  }
  problem_id = ntohl(*(long*)(header + 1));
  version = ntohl(*(long*)(header + 5));

  return 0;
}

int saveFile(int communicate_socket,
             const string& filename) {
  int length;
  if (socket_read(communicate_socket, length, sizeof(length)) <
      sizeof(length)) {
    LOG(ERROR) << "Cannot get file length of " << filename;
    sendReply(communicate_socket, INVALID_FILE_SIZE);
    return -1;
  }
  sendReply(communicate_socket, READY);
  
  int file = open(filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
  if (file == -1) {
    LOG(ERROR) << "Cannot create file " << filename;
    sendReply(communicate_socket, SYSTEM_ERROR);
    return -1;
  }
  static char buffer[4096];
  while (length) {
    size_t count = min(length, sizeof(buffer));
    if (socket_read(communicate_socket, buffer, count) < count) {
      LOG(ERROR) << "Cannot read file from " << communicate_socket;
      sendReply(communicate_socket, SYSTEM_ERROR);
      return -1;
    }
    if (socket_write(file, buffer, count) == -1) {
      LOG(ERROR) << "Cannot write file to " << filename;
      sendReply(communicate_socket, SYSTEM_ERROR);
      return -1;
    }
    length -= count;
  }
  close(file);
  return 0;
}

int saveData(int communicate_socket, int problem_id, int version) {
  string problem_dir = FLAGS_root_dir +
                       stringPrintf("/%d/%d", problem_id, version);
  string temp_file = problem_dir + "/data.tar.gz";
  mkdirRecursive(problem_dir);
  if (saveFile(communicate_socket, temp_file) == -1) {
    return -1;
  }
  return 0;
}

int getProblemLimit(int communicate_socket,
                    int& test_case,
                    int& time_limit,
                    int& case_time_limit,
                    int& memory_limit) {
  unsigned char header[13];
  int num = socket_read(communicate_socket, header, sizeof(header));
  if (num < sizeof(header))
    return -1;

  test_case = header[0];
  time_limit = ntohl(*(long*)(header + 1));
  case_time_limit = ntohl(*(long*)(header + 5));
  memory_limit = ntohl(*(long*)(header + 9));

  return 0;
}

void process(int communicate_socket) {
  string source_suffix;
  int problem_id;
  int version;
  if (getHeader(communicate_socket,
                source_suffix,
                problem_id,
                version) == -1)
    return;

  LOG(INFO) << "Start to judge " << problem_id << "(" << version << ")."
            << source_suffix;

  string working_root = FLAGS_root_dir + stringPrintf("/working/%d", getpid());
  string binary_filename = working_dir + "/prog";
  string source_filename = binary_filename + "." + source_suffix;
  string program_output_filename = working_dir + "/prog.out";

  LOG(INFO) << "Start to save source file";
  // Get source file
  sendReply(communicate_socket, READY);
  if (saveFile(communicate_socket, source_filename) == -1) {
    LOG(ERROR) << "Cannot get source file";
    return;
  }
  LOG(INFO) << "Successful get source file as " << source_filename;

  // Get input/output files
  string problem_dir = FLAGS_root_dir +
                       stringPrintf("/prob/%d/%d", problem_id, version);
  if (access(problem_dir, F_OK) == 0) {
    // Already exsist data
    sendReply(communicate_socket, DATA_EXSIST);
    LOG(INFO) << "Data exsisted";
  } else {
    LOG(INFO) << "Start to sync data";
    sendReply(communicate_socket, READY);
    if (saveData(communicate_socket, problem_id, version) == -1) {
      LOG(ERROR) << "Cannot sync data";
      return;
    }
    LOG(INFO) << "Data sync successful";
  }

  // Get limits
  sendReply(communicate_socket, READY);
  int test_case;
  int time_limit;
  int case_time_limit;
  int memory_limit;
  if (getProblemLimit(communicate_socket,
                      test_case,
                      time_limit,
                      case_time_limit,
                      memory_limit) == -1)
    return;

  if (doCompile(communicate_socket, source_filename) == -1)
    return;

  for

  sendReply(communicate_socket, READY);
}

int terminated = 0;
int socket_broken = 0;

void sigterm_handler(int signal) {
  terminated = 1;
}

void sigpipe_handler(int signal) {
  socket_broken = 1;
}

void sigchld_handler(int signal) {
  pid_t pid;
  while ((pid = waitpid(-1, NULL, WNOHANG)) > 0 || pid < 0 && errno == EINTR)
    ;
}

int main(int argc, char* argv[]) {
  if (parseFlags(argc, argv) < 0) {
    LOG(SYS_ERROR) << "Cannot parse flags.";
    return 1;
  }

  // Run in daemonize mode
  if (FLAGS_daemon) {
    
  }

  // change working directory
  if (chdir(FLAGS_root_dir.c_str()) < 0) {
    LOG(SYS_ERROR) << strerror(errno) << endl
                   << "Fail to change working dir to " << FLAGS_root_dir;
    return 1;
  }
  char path[MAX_PATH_LENGTH + 1];
  if (getcwd(path, sizeof(path)) == NULL) {
    LOG(SYS_ERROR) << "Cannot get current working directory";
    return 1;
  }
  FLAGS_root_dir = path;

  string working_root =
    stringPrintf("%s/working/%d", FLAGS_root_dir.c_str(), getpid());

  if (mkdir(working_root.c_str(), 0777) < 0) {
    LOG(SYS_ERROR) << "Cannot create directory " << working_root;
    return 1;
  }

  // sigaction SIGCHLD, SIGTERM, SIGPIPE
  installSignalHandler(SIGCHLD, sigchld_handler);
  installSignalHandler(SIGTERM, sigterm_handler);
  installSignalHandler(SIGPIPE, sigpipe_handler);

  // connect to server
  int communicate_socket =
    connectToServer(FLAGS_server_address, FLAGS_server_port);
  if (communicate_socket == -1)
    return -1;

  while (!terminated && !socket_broken) {
    if (process(communicate_socket) == -1)
      break;
    system(stringPrintf("rm -f %s/*", working_dir).c_str());
  }
  system(stringPrintf("rm -rf %s", working_dir).c_str());

  close(communicate_socket);
  return 0;
}

