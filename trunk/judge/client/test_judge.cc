// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <unistd.h>

#include "base/flags.h"
#include "base/judge_result.h"
#include "base/logging.h"
#include "base/util.h"

#include "judge/client/judge.h"
#include "judge/client/trace.h"
#include "judge/client/utils.h"
#include "judge/client/run.h"

using namespace std;

DEFINE_FLAGS(int, uid, "uid for run SPJ");
DEFINE_FLAGS(int, gid, "gid for run SPJ");

DEFINE_FLAGS(string, server_address, "server address");
DEFINE_FLAGS(int, server_port, "server port");

int main(int argc, char* argv[]) {
  if (parseFlags(argc, argv)) {
    LOG(SYS_ERROR) << "Unable to parse flags";
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
  installHandlers();
  LOG(INFO) << "Install handlers finished";

  string test_file_dir = "/tmp/testdata/";
  string spj_filename = test_file_dir + "spj";
  string spj_input_filename = test_file_dir + "spj_in.txt";
  string accepted_filename = test_file_dir + "spj_ac.txt";
  string presentation_error_filename = test_file_dir + "spj_pe.txt";
  string wrong_answer_filename = test_file_dir + "spj_wa.txt";
  LOG(SYS_ERROR) << "Detect Position";

  bool pass = true;

  if (doRun(communicate_socket,
            spj_filename,
            "cc",
            spj_input_filename,
            "/tmp/testdata/spj.out",
            1,
            65535,
            65535)) {
    LOG(ERROR) << "Failed on Run " << spj_filename;
  }

  if (doJudge(communicate_socket,
              spj_input_filename,
              "",
              accepted_filename,
              spj_filename) == ACCEPTED) {
    LOG(INFO) << "Pass SPJ Accepted Test";
  } else {
    LOG(ERROR) << "Failed on SPJ Accepted Test";
    pass = false;
  }

  if (doJudge(communicate_socket,
              spj_input_filename,
              "",
              presentation_error_filename,
              spj_filename) == PRESENTATION_ERROR) {
    LOG(INFO) << "Pass SPJ Presentation Error Test";
  } else {
    LOG(ERROR) << "Failed on SPJ Presentation Error Test";
    pass = false;
  }

  if (doJudge(communicate_socket,
              spj_input_filename,
              "",
              wrong_answer_filename,
              spj_filename) == WRONG_ANSWER) {
    LOG(INFO) << "Pass SPJ Wrong Answer Test";
  } else {
    LOG(ERROR) << "Failed on SPJ Wrong Answer Test";
    pass = false;
  }

  if (pass)
    LOG(INFO) << "PASSED ALL TESTS";
  else
    LOG(ERROR) << "FAILED ON SOME TEST(S)";

  return 0;
}

