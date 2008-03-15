// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>

#include "base/flags.h"
#include "base/logging.h"
#include "base/util.h"

#include "judge/client/client.h"
#include "judge/client/compile.h"
#include "judge/client/judge.h"
#include "judge/client/run.h"
#include "judge/client/util.h"

using namespace std;

DEFINE_FLAGS(string, root_dir, "The root working dir");

void process(int communicate_socket) {
}

int terminated = 0;

void sigterm_handler(int siginal) {
  terminated = 1;
}

int main(int argc, char* argv[]) {
  if (parseFlags(argc, argv) < 0) {
    return 1;
  }
  if (chdir(FLAGS_root_dir.c_str()) < 0) {
    LOG(SYS_ERROR) << strerror(errno) << endl
                   << "Fail to change working dir to " << FLAGS_root_dir;
    return 1;
  }

  sigset_t mask;
  sigemptyset(&mask);
  install_signal_handler(SIGTERM, sigterm_handler, 0, mask);

  int communicate_socket = socket(AF_INET, SOCK_STREAM, 0);
  struct socketaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(ARG_server_port);
  if (inet_pton(AF_INET,
                ARG_server_address.c_str(),
                &server_address.sin_addr) <= 0) {
    LOG(SYS_ERROR) << "Invalid Server Address: " << ARG_server_address;
    return 1;
  }
  if (connect(communicate_socket,
              (const sockaddr*)&server_address,
              sizeof(server_address)) < 0) {
    LOG(SYS_ERROR) << "Fail to connect to "
                   << ARG_server_address << ":" << ARG_server_port;
    return 1;
  }

  while (!terminated) {
    process(communicate_socket);
  }

  close(communicate_socket);
  return 0;
}

