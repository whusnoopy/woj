// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <sstream>

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "base/flags.h"
#include "base/logging.h"
#include "base/judge_result.h"
#include "base/util.h"

#include "judge/client/client.h"
#include "judge/client/compile.h"
#include "judge/client/judge.h"
#include "judge/client/result.h"
#include "judge/client/run.h"
#include "judge/client/trace.h"
#include "judge/client/utils.h"

using namespace std;

DECLARE_FLAGS(string, root_dir);

DEFINE_FLAGS(string, support_lang, "Programming lanugages supported by this "
                                   "judge, seperate by ','");

DEFINE_FLAGS(int, uid, "The uid for judge run program and special judge");
DEFINE_FLAGS(int, gid, "The gid for judge run program and special judge");

DEFINE_FLAGS(string, server_address, "The server address this judge should "
                                     "connect to");
DEFINE_FLAGS(int, server_port, "The server port this judge should connect to");

DEFINE_OPTIONAL_FLAGS(bool, daemon, true, "Run this judge daemonize or not");

bool isSupportedSourceFileType(const string& source_suffix) {
  static vector<string> supported_languages;
  if (supported_languages.size() == 0) {
    int p = 0;
    int q = 0;
    while (p < FLAGS_support_lang.length()) {
      q = FLAGS_support_lang.find(',', p);
      if (q == -1)
        break;
      supported_languages.push_back(FLAGS_support_lang.substr(p, q - p));
      p = q + 1;
    }
    supported_languages.push_back(FLAGS_support_lang.substr(p));
  }
  return find(supported_languages.begin(),
              supported_languages.end(),
              source_suffix) != supported_languages.end();
}

int getHeader(int communicate_socket,
              string& source_suffix,
              int& problem_id,
              int& version) {
  // Wait for 5s, if there is no header recieved, return to wait for terminaled
  // and socket_broken
  

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
  if (socket_read(communicate_socket, &length, sizeof(length)) <
      sizeof(length)) {
    LOG(ERROR) << "Cannot get file length of " << filename;
    sendReply(communicate_socket, INVALID_FILE_SIZE);
    return -1;
  }
  length = ntohl(length);
  LOG(DEBUG) << "Get file length = " << length;
  sendReply(communicate_socket, READY);
  
  int file = open(filename.c_str(), O_RDWR | O_CREAT | O_TRUNC, 0644);
  if (file == -1) {
    LOG(ERROR) << "Cannot create file " << filename;
    sendReply(communicate_socket, SYSTEM_ERROR);
    return -1;
  }
  static char buffer[4096];
  while (length) {
    size_t count = (length > sizeof(buffer) ? sizeof(buffer) : length);
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
    LOG(DEBUG) << "Success get " << count << " Bytes already, "
               << length << " Bytes left";
    LOG(DEBUG) << "Write " << buffer <<  " to "
               << file << " (" << filename << ")";
  }
  close(file);
  return 0;
}

int saveData(int communicate_socket, int problem_id, int version) {
  string problem_dir = FLAGS_root_dir +
                       stringPrintf("/prob/%d/%d", problem_id, version);
  string temp_file = problem_dir + "/data.tar.gz";
  mkdirRecursive(problem_dir, 0755);
  if (saveFile(communicate_socket, temp_file) == -1) {
    return -1;
  }

  string extract_command = "tar xzf " + temp_file + " -C " + problem_dir;
  system(extract_command.c_str());
  string remove_command = "rm " + temp_file;
  system(remove_command.c_str());
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
  if (test_case == 0 || test_case > MAX_TEST_CASE) {
    LOG(ERROR) << "Invalid test case number";
    sendReply(communicate_socket, SYSTEM_ERROR);
    return -1;
  }
  time_limit = ntohl(*(long*)(header + 1));
  if (time_limit == 0 || time_limit > MAX_TIME_LIMIT) {
    LOG(ERROR) << "Invalid time limit";
    sendReply(communicate_socket, SYSTEM_ERROR);
    return -1;
  }
  case_time_limit = ntohl(*(long*)(header + 5));
  if (case_time_limit == 0 || case_time_limit > MAX_CASE_TIME_LIMIT) {
    LOG(ERROR) << "Invalid case time limit";
    sendReply(communicate_socket, SYSTEM_ERROR);
    return -1;
  }
  memory_limit = ntohl(*(long*)(header + 9));
  if (memory_limit == 0 || memory_limit > MAX_MEMORY_LIMIT) {
    LOG(ERROR) << "Invalid memory limit";
    sendReply(communicate_socket, SYSTEM_ERROR);
    return -1;
  }

  return 0;
}

int sendResultMessage(int communicate_socket,
                      char result, int time_, int memory_) {
  static char message[9];
  message[0] = result;
  *(unsigned int*)(message + 1) = htonl((unsigned int)(time_));
  *(unsigned int*)(message + 5) = htonl((unsigned int)(memory_));
  if (socket_write(communicate_socket, message, sizeof(message)) == -1) {
    LOG(ERROR) << "Fail to send running message back";
    return -1;
  }
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

  string working_dir = FLAGS_root_dir + stringPrintf("/working/%d", getpid());
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
  if (access(problem_dir.c_str(), F_OK) == 0) {
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
  // Check whether special judge
  string special_judge_filename = problem_dir + "/spj.cc";
  if (access(special_judge_filename.c_str(), F_OK) == 0) {
    doCompile(communicate_socket, special_judge_filename);
    special_judge_filename = problem_dir + "/spj";
  } else {
    special_judge_filename = "";
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
  if (case_time_limit > time_limit)
    case_time_limit = time_limit;
  
  // Compile process
  sendReply(communicate_socket, READY);
  if (doCompile(communicate_socket, source_filename) == -1)
    return;

  JudgeResult judge_result;
  for (int i = 1; i <= test_case; ++i) {
    string standard_input_filename = problem_dir + stringPrintf("/%d.in", i);
    string standard_output_filename = problem_dir + stringPrintf("/%d.out", i);

    judge_result.updateResult(doRun(communicate_socket,
                              binary_filename,
                              source_suffix,
                              standard_input_filename,
                              program_output_filename,
                              case_time_limit,
                              memory_limit,
                              MAX_OUTPUT_LIMIT));
    if (judge_result.getResult() != 0) {
      break;
    }
    if (judge_result.getTime() > time_limit) {
      judge_result.onTimeLimitExceeded(time_limit);
      break;
    }

    judge_result.updateResult(doJudge(communicate_socket,
                                      standard_input_filename,
                                      standard_output_filename,
                                      program_output_filename,
                                      special_judge_filename));
    if (judge_result.getResult() != ACCEPTED) {
      break;
    }
  }

  sendResultMessage(communicate_socket,
                    judge_result.getResult(),
                    judge_result.getTime(),
                    judge_result.getMemory());
  return;
}

int alreadyRunning() {
  int file = open(stringPrintf("%s/judge.pid", FLAGS_root_dir.c_str()).c_str(),
                  O_RDWR | O_CREAT,
                  0644);
  if (file < 0) {
    LOG(SYS_ERROR) << "Fail to open judge.pid";
    exit(1);
  }
  if (lockFile(file, F_SETLK) == -1) {
    if (errno == EACCES || errno == EAGAIN) {
      close(file);
      return 1;
    } else {
      LOG(SYS_ERROR) << "Fail to lock judge.pid";
      exit(1);
    }
  }
  ftruncate(file, 0);
  char buffer[20];
  sprintf(buffer, "%ld", (long)(getpid()));
  socket_write(file, buffer, strlen(buffer) + 1);
  return 0;
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
    // daemon();
    if (alreadyRunning()) {
      LOG(ERROR) << "Another instance of judge_client exists";
      return 1;
    }
  }

  // change working directory
  if (chdir(FLAGS_root_dir.c_str()) < 0) {
    LOG(SYS_ERROR) << strerror(errno) << endl
                   << "Fail to change working dir to " << FLAGS_root_dir;
    return 1;
  }
  LOG(DEBUG) << "Change root_dir successful";

  char path[MAX_PATH_LENGTH + 1];
  if (getcwd(path, sizeof(path)) == NULL) {
    LOG(SYS_ERROR) << "Cannot get current working directory";
    return 1;
  }
  FLAGS_root_dir = path;

  string working_root =
    stringPrintf("%s/working/%d", FLAGS_root_dir.c_str(), getpid());

  if (mkdirRecursive(working_root.c_str(), 0777) < 0) {
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
  if (communicate_socket == -1) {
    system(stringPrintf("rm -rf %s", working_root.c_str()).c_str());
    return -1;
  }

  while (!terminated && !socket_broken) {
    process(communicate_socket);
//    system(stringPrintf("rm -f %s/*", working_root.c_str()).c_str());
  }
  system(stringPrintf("rm -rf %s", working_root.c_str()).c_str());

  close(communicate_socket);
  return 0;
}

