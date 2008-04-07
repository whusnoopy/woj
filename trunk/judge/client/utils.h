// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _JUDGE_CLIENT_UTILS_H__
#define _JUDGE_CLIENT_UTILS_H__

int setLimit(int resource, unsigned int limit);

int readTime(int pid);

int readMemory(int pid);

struct RunInfo {
  RunInfo()
    : file_stdin(0), file_stdout(0), file_stderr(0),
      stdin_filename(NULL), stdout_filename(NULL), stderr_filename(NULL),
      uid(0), gid(0),
      time_limit(0), memory_limit(0), output_limit(0),
      proc_limit(0), file_limit(0),
      trace(false),
      working_dir(NULL) {};

  int file_stdin;
  int file_stdout;
  int file_stderr;

  const char* stdin_filename;
  const char* stdout_filename;
  const char* stderr_filename;

  int uid;
  int gid;

  int time_limit;
  int memory_limit;
  int output_limit;

  int proc_limit;
  int file_limit;

  bool trace;

  const char* working_dir;
};

int createProcess(const char* commands[], const RunInfo& run_info);

int createShellProcess(const char* command, const RunInfo& run_info);

sighandler_t installSignalHandler(int signal, sighandler_t handler);

sighandler_t installSignalHandler(int signal, sighandler_t handler, int flags);

sighandler_t installSignalHandler(int signal,
                                  sighandler_t handler,
                                  int flags,
                                  sigset_t mask);

int connectToServer(const string& address, int port);

int lockFile(int file, int cmd);

#endif

