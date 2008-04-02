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

DEFINE_FLAGS(string, server_address, "The server address");
DEFINE_FLAGS(int, server_port, "The server port");

DEFINE_FLAGS(int, uid, "The uid for judge run program and special judge");
DEFINE_FLAGS(int, gid, "The gid for judge run program and special judge");

void process(int communicate_socket) {
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

  // sigaction SIGCHLD, SIGTERM, SIGPIPE
  installSignalHandler(SIGCHLD, sigchld_handler);
  installSignalHandler(SIGTERM, sigterm_handler);
  installSignalHandler(SIGPIPE, sigpipe_handler);

  // connect to server
  sigset_t mask;
  sigemptyset(&mask);
  installSignalHandler(SIGTERM, sigterm_handler, 0, mask);

  int communicate_socket = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(FLAGS_server_port);
  if (inet_pton(AF_INET,
                FLAGS_server_address.c_str(),
                &server_address.sin_addr) <= 0) {
    LOG(SYS_ERROR) << "Invalid Server Address: " << FLAGS_server_address;
    return 1;
  }
  if (connect(communicate_socket,
              (const sockaddr*)&server_address,
              sizeof(server_address)) < 0) {
    LOG(SYS_ERROR) << "Fail to connect to "
                   << FLAGS_server_address << ":" << FLAGS_server_port;
    return 1;
  }

  while (!terminated) {
    process(communicate_socket);
  }

  close(communicate_socket);
  return 0;
}

