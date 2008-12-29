// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "base/flags.h"
#include "base/logging.h"
#include "base/util.h"
#include "base/judge_result.h"

#include "judge/client/result.h"
#include "judge/client/run.h"
#include "judge/client/trace.h"
#include "judge/client/utils.h"

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

  installSignalHandler(SIGPIPE, SIG_IGN);

  const string test_dir = "/tmp/testdata/";
  const string input_filename = test_dir + "spj_in.txt";
  const string program_output_filename = test_dir + "pro_out.txt";

  const string ac_file = test_dir + "ac";

  if (doRun(communicate_socket,
            test_dir,
            ac_file,
            "cc",
            input_filename,
            program_output_filename,
            1,
            65535,
            65535)) {
    LOG(ERROR) << "FAILED Run '" << ac_file << "' Test";
    return -1;
  }
  sendReply(communicate_socket, 0);
  LOG(INFO) << "PASS Run '" << ac_file << "' Test";

  const string java_file = test_dir + "Main.class";

  if (doRun(communicate_socket,
            test_dir,
            java_file,
            "cc",
            input_filename,
            program_output_filename,
            1,
            65535,
            65535)) {
    LOG(ERROR) << "FAILED Run '" << java_file << "' Test";
    return -1;
  }
  sendReply(communicate_socket, 0);
  LOG(INFO) << "PASS Run '" << java_file << "' Test";

  close(communicate_socket);

  LOG(INFO) << "PASSED ALL TESTS";
  return 0;
}

