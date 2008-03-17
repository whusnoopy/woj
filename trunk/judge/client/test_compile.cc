// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "base/flags.h"
#include "base/logging.h"
#include "base/util.h"

#include "base/judge_result.h"

#include "judge/client/compile.h"

using namespace std;

DEFINE_FLAGS(string, server_address, "server address");
DEFINE_FLAGS(int, server_port, "server port");

int main(int argc, char* argv[]) {
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

  return 0;
}

