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

DEFINE_FLAGS(int, uid, "");
DEFINE_FLAGS(int, gid, "");

int main(int argc, char* argv[]) {
  if (parseFlags(argc, argv)) {
    LOG(SYS_ERROR) << "Cannot parse flags!";
    return -1;
  }

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

  const string ac_file = "testdata/ac.cc";
  if (doCompile(communicate_socket, ac_file)) {
    LOG(ERROR) << "FAILED Compile '" << ac_file << "' Test";
    return -1;
  }
  sendReply(communicate_socket, 0);
  LOG(INFO) << "PASS Compile '" << ac_file << "' Test";

  const string ce_file = "testdata/ce.cc";
  if (doCompile(communicate_socket, ce_file) == 0) {
    LOG(ERROR) << "FAILED Compile '" << ce_file << "' Test";
    return -1;
  }
  LOG(INFO) << "PASS Compile '" << ce_file << "' Test";

  close(communicate_socket);

  LOG(INFO) << "PASSED ALL TESTS";
  return 0;
}

