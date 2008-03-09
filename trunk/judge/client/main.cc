// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>

#include "args.h"
#include "client.h"
#include "logging.h"
#include "util.h"

void process(int communicate_socket) {
}

int terminated = 0;

void sigterm_handler(int siginal) {
  terminated = 1;
}

int main(int argc, char* argv[]) {

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

