// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client.h"
#include "logging.h"

#include "judge.h"

using namespace std;

int main(int argc, char* argv[]) {
  string test_file_directory = "./testdata/";
  string standard_test_file_name = test_file_directory + "test_judge.txt";
  string compare_accepted_file_name = test_file_directory + "test_judge1.txt";
  string compare_presentation_error_file_name = test_file_directory +
                                                "test_judge2.txt";
  string compare_wrong_answer_file_name = test_file_directory +
                                          "test_judge3.txt";


  string ARG_server_address = "127.0.0.1";
  int ARG_server_port = 631;
  int communicate_socket = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_address;
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

  if (doJudge(communicate_socket,
              "",
              standard_test_file_name,
              compare_accepted_file_name,
              "") == ACCEPTED) {
    LOG(INFO) << "Pass Accepted Test";
  } else {
    LOG(ERROR) << "Failed on Accepted Test";
  }

  if (doJudge(communicate_socket,
              "",
              standard_test_file_name,
              compare_presentation_error_file_name,
              "") == PRESENTATION_ERROR) {
    LOG(INFO) << "Pass Presentation Error Test";
  } else {
    LOG(ERROR) << "Failed on Presentation Error Test";
  }

  if (doJudge(communicate_socket,
              "",
              standard_test_file_name,
              compare_wrong_answer_file_name,
              "") == WRONG_ANSWER) {
    LOG(INFO) << "Pass Wrong Answer Test";
  } else {
    LOG(ERROR) << "Failed on Wrong Answer Test";
  }

  return 0;
}

