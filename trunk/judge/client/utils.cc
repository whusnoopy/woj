// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/ptrace.h>
#include <sys/time.h>
#include <sys/wait.h>

#include "base/flags.h"
#include "base/logging.h"
#include "base/util.h"

#include "judge/client/client.h"
#include "judge/client/utils.h"

DECLARE_FLAGS(string, root_dir);

int setLimit(int resource, unsigned int limit) {
  struct rlimit t;
  t.rlim_max = limit + 1;
  t.rlim_cur = limit;
  if (setrlimit(resource, &t) == -1) {
    LOG(SYS_ERROR) << "Can't set rlimit to " << resource;
    return -1;
  }
  return 0;
}

int readTime(int pid) {
  char buffer[64];
  sprintf(buffer, "/proc/%d/stat", pid);
  FILE* file = fopen(buffer, "r");
  if (file == NULL)
    return -1;
  int utime, stime;
  while (fgetc(file) != ')');
  fgetc(file);
  fscanf(file,
         "%*c "
         "%*d %*d %*d %*d %*d "
         "%*u %*u %*u %*u %*u "
         "%d %d",
         &utime, &stime);
  fclose(file);
  static int clktck = 0;
  if (clktck == 0) {
    clktck = sysconf(_SC_CLK_TCK);
  }
  return static_cast<int>((utime + stime) * 1000.0 / clktck);
}

int readMemory(int pid) {
  char buffer[64];
  sprintf(buffer, "/proc/%d/status", pid);
  FILE* file = fopen(buffer, "r");
  if (file == NULL) {
    return -1;
  }
  int vm_peak = 0, vm_size = 0, vm_exe = 0, vm_lib = 0;
  while (fgets(buffer, 32, file)) {
    if (strncmp(buffer, "VmPeak:", 7) == 0) {
      sscanf(buffer + 7, "%d", &vm_peak);
    } else if (strncmp(buffer, "VmSize:", 7) == 0) {
      sscanf(buffer + 7, "%d", &vm_size);
    } else if (strncmp(buffer, "VmExe:", 6) == 0) {
      sscanf(buffer + 6, "%d", &vm_exe);
    } else if (strncmp(buffer, "VmLib:", 6) == 0) {
      sscanf(buffer + 6, "%d", &vm_lib);
    }
  }
  if (vm_peak)
    vm_size = vm_peak;
  return vm_size - vm_exe - vm_lib;
}

static unsigned int msec_alarm(int m_seconds) {
  struct itimerval old_alarm_time, new_alarm_time;
  new_alarm_time.it_interval.tv_usec = 0;
  new_alarm_time.it_interval.tv_sec = 0;
  new_alarm_time.it_value.tv_usec = m_seconds * 1000;
  new_alarm_time.it_value.tv_sec = 0;

  if (setitimer(ITIMER_REAL, &new_alarm_time, &old_alarm_time) < 0)
    return 0;
  
  return old_alarm_time.it_value.tv_sec;
}

int createProcess(const char* commands[], const RunInfo& run_info) {
  string command_line = commands[1];
  for (int i = 2; commands[i]; ++i)
    command_line = command_line + " " + commands[i];

  LOG(DEBUG) << "Create process by command : \"" << command_line << "\"";

  const char* filename[] = {run_info.stdin_filename,
                            run_info.stdout_filename,
                            run_info.stderr_filename};
  int mode[] = {O_RDONLY, O_RDWR | O_CREAT | O_TRUNC, O_RDWR | O_CREAT | O_TRUNC};
  int file[] = {run_info.file_stdin,
                run_info.file_stdout,
                run_info.file_stderr};
  for (int i = 0; i < 3; i++) {
    if (filename[i]) {
      file[i] = open(filename[i], mode[i], 0777);
      if (file[i] == -1) {
        LOG(SYS_ERROR) << "Fail to open " << filename[i];
        for (int j = 0; j < i; j++) {
          if (filename[j])
            close(file[i]);
        }
        return -1;
      }
      LOG(DEBUG) << "Open " << filename[i] << " to " << file[i];
    }
  }
  LOG(DEBUG) << "Success Opened stdin/stdout/stderr to "
             << file[0] << "/" << file[1] << "/" << file[2];

  LOG(DEBUG) << "Time limit is " << run_info.time_limit << "ms";

  int pid = fork();
  if (pid < 0) {
    LOG(SYS_ERROR) << "Unable to fork";
    return -1;
  } else if (pid > 0) {
    return pid;
  }
  for (int i = 0; i < 3; i++) {
    if (file[i]) {
      if (dup2(file[i], i) == -1) {
        LOG(SYS_ERROR) << "Fail to dup " << file[i] << " to " << i;
        raise(SIGKILL);
      }
      LOG(DEBUG) << "Dup " << file[i] << " to " << i;
      close(file[i]);
    }
  }
  for (int i = 3; i < 100; i++)
    close(i);
  LOG(DEBUG) << "close fds successful";
  if (run_info.time_limit) {
    if (setLimit(RLIMIT_CPU, run_info.time_limit / 1000 ) == -1) {
      LOG(SYS_ERROR) << "Fail to set cpu limit to "
                     << run_info.time_limit << "ms";
      raise(SIGKILL);
    }
    if (msec_alarm(run_info.time_limit * 2) == -1) {
      LOG(SYS_ERROR) << "Fail to set clock limit to "
                     << run_info.time_limit << "ms";
      raise(SIGKILL);
    }
    LOG(DEBUG) << "set time limit successful";
  }
  if (run_info.memory_limit) {
    if (setLimit(RLIMIT_DATA, run_info.memory_limit * 1024) == -1) {
      LOG(SYS_ERROR) << "Fail to set memory limit to "
                     << run_info.memory_limit << "KiB";
      raise(SIGKILL);
    }
    LOG(DEBUG) << "set memory limit successful";
  }
  if (run_info.output_limit) {
    if (setLimit(RLIMIT_FSIZE, run_info.output_limit * 1024) == -1) {
      LOG(SYS_ERROR) << "Fail to set output limit to "
                     << run_info.output_limit << "KiB";
      raise(SIGKILL);
    }
    LOG(DEBUG) << "set output limit successful";
  }
  if (run_info.file_limit) {
    if (setLimit(RLIMIT_NOFILE, run_info.file_limit) == -1) {
      LOG(SYS_ERROR) << "Fail to set file limit to " << run_info.file_limit;
      raise(SIGKILL);
    }
    LOG(DEBUG) << "set number of file limit successful";
  }
  if (run_info.working_dir) {
    if (chdir(run_info.working_dir) == -1) {
      LOG(SYS_ERROR) << "Fail to change working directory to "
                     << run_info.working_dir;
      raise(SIGKILL);
    }
    LOG(DEBUG) << "change working directory successful";
  }
  if (run_info.gid) {
    if (setgid(run_info.gid) == -1) {
      LOG(SYS_ERROR) << "Fail to set gid to " << run_info.gid;
      raise(SIGKILL);
    }
    LOG(DEBUG) << "set gid successful";
  }
  if (run_info.uid) {
    if (setuid(run_info.uid) == -1) {
      LOG(SYS_ERROR) << "Fail to set uid to " << run_info.uid;
      raise(SIGKILL);
    }
    LOG(DEBUG) << "set uid successful";
  }
  if (run_info.proc_limit) {
    if (setLimit(RLIMIT_NPROC, run_info.proc_limit) == -1) {
      LOG(SYS_ERROR) << "Fail to set process limit to " << run_info.proc_limit;
      raise(SIGKILL);
    }
    LOG(DEBUG) << "set number of process limit successful";
  }
  if (run_info.trace) {
    if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) == -1) {
      LOG(SYS_ERROR) << "Fail to trace";
      raise(SIGKILL);
    }
    LOG(DEBUG) << "set trace successful";
  }
  LOG(DEBUG) << "Set run_info successful";
  if (execv(commands[0], (char**)(commands + 1)) == -1) {
    LOG(SYS_ERROR) << "Fail to execute command '" << commands[0] << "'";
    raise(SIGKILL);
  }
  return -1;
}

int createShellProcess(const char* command, const RunInfo& run_info) {
  const char* commands[] = {"/bin/bash", "bash", "-c", command, NULL};
  return createProcess(commands, run_info);
}

sighandler_t installSignalHandler(int signal, sighandler_t handler) {
  return installSignalHandler(signal, handler, 0);
}

sighandler_t installSignalHandler(int signal, sighandler_t handler, int flags) {
  sigset_t mask;
  sigemptyset(&mask);
  return installSignalHandler(signal, handler, flags, mask);
}

sighandler_t installSignalHandler(int signal,
                                  sighandler_t handler,
                                  int flags,
                                  sigset_t mask) {
  struct sigaction act, old_act;
  act.sa_handler = handler;
  act.sa_mask = mask;
  act.sa_flags = flags;
  if (sigaction(signal, &act, &old_act) < 0)
    return SIG_ERR;
  return old_act.sa_handler;
}

int connectToServer(const string& address, int port) {
  int communicate_socket = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  if (inet_pton(AF_INET, address.c_str(), &server_address.sin_addr) <= 0) {
    LOG(SYS_ERROR) << "Invalid Server Address: " << address;
    return -1;
  }
  if (connect(communicate_socket,
              (const sockaddr*)&server_address,
              sizeof(server_address)) < 0) {
    LOG(SYS_ERROR) << "Fail to connect to " << address << ":" << port;
    return -1;
  }
  return communicate_socket;
}

int lockFile(int file, int cmd) {
  struct flock lock;
  lock.l_type = F_WRLCK;
  lock.l_start = 0;
  lock.l_whence = SEEK_SET;
  lock.l_len = 0;
  return fcntl(file, cmd, &lock);
}

int convertFileFormat(const string& filename) {
  RunInfo run_info;
  run_info.time_limit = CONVERT_FILE_TIME_LIMIT;
  string command = FLAGS_root_dir + "/bin/convert.sh " + filename;
  pid_t pid = createShellProcess(command.c_str(), run_info);
  if (pid < 0) {
    LOG(ERROR) << "Fail to create shell process to compile";
    return -1;
  }
  LOG(DEBUG) << "Create process " << pid << " to convert";
  int status;
  waitpid(pid, &status, 0);
  if (WEXITSTATUS(status)) {
    LOG(ERROR) << "Unknown error during convert file with exit status = "
               << status << "(WEXITSTATUS = " << WEXITSTATUS(status) << ")";
    return -1;
  }
  return 0;
}

