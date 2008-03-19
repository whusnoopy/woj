// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>

#include "base/logging.h"
#include "base/util.h"

#include "judge/client/utils.h"

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

int createProcess(const char* commands[], const RunInfo& run_info) {
  string command_line = commands[1];
  for (int i = 2; commands[i]; ++i)
    command_line = command_line + " " + commands[i];
  LOG(INFO) << "Create process by command : \"" << command_line << "\"";

  const char* filename[] = {run_info.stdin_filename,
                            run_info.stdout_filename,
                            run_info.stderr_filename};
  int mode[] = {O_RDONLY, O_RDWR | O_CREAT | O_TRUNC, O_RDWR};
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
    }
  }
  LOG(INFO) << "Success Opened stdin/stdout/stderr";
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
      close(file[i]);
    }
  }
  for (int i = 3; i < 100; i++)
    close(i);
  if (run_info.time_limit) {
    if (setLimit(RLIMIT_CPU, run_info.time_limit) == -1) {
      LOG(SYS_ERROR) << "Fail to set cpu limit to "
                     << run_info.time_limit << "s";
      raise(SIGKILL);
    }
  }
  if (run_info.memory_limit) {
    if (setLimit(RLIMIT_DATA, run_info.memory_limit * 1024) == -1) {
      LOG(SYS_ERROR) << "Fail to set memory limit to "
                     << run_info.memory_limit << "KiB";
      raise(SIGKILL);
    }
  }
  if (run_info.output_limit) {
    if (setLimit(RLIMIT_FSIZE, run_info.output_limit * 1024) == -1) {
      LOG(SYS_ERROR) << "Fail to set output limit to "
                     << run_info.output_limit << "KiB";
      raise(SIGKILL);
    }
  }
  if (run_info.file_limit) {
    if (setLimit(RLIMIT_NOFILE, run_info.file_limit) == -1) {
      LOG(SYS_ERROR) << "Fail to set file limit to " << run_info.file_limit;
      raise(SIGKILL);
    }
  }
  if (run_info.working_dir) {
    if (chdir(run_info.working_dir) == -1) {
      LOG(SYS_ERROR) << "Fail to change working directory to "
                     << run_info.working_dir;
      raise(SIGKILL);
    }
  }
  if (run_info.gid) {
    if (setgid(run_info.gid) == -1) {
      LOG(SYS_ERROR) << "Fail to set gid to " << run_info.gid;
      raise(SIGKILL);
    }
  }
  if (run_info.gid) {
    if (setuid(run_info.uid) == -1) {
      LOG(SYS_ERROR) << "Fail to set uid to " << run_info.uid;
      raise(SIGKILL);
    }
  }
  if (run_info.proc_limit) {
    if (setLimit(RLIMIT_NPROC, run_info.proc_limit) == -1) {
      LOG(SYS_ERROR) << "Fail to set process limit to " << run_info.proc_limit;
      raise(SIGKILL);
    }
  }
  if (run_info.trace) {
  }
  if (execv(commands[0], (char**)(commands + 1)) == -1) {
    LOG(SYS_ERROR) << "Fail to execute command '" << commands[0] << "'";
    raise(SIGKILL);
  }
  return -1;
}

int createShellProcess(const char* command, const RunInfo& run_info) {
  const char* commands[] = {"/bin/sh", "sh", "-c", command, NULL};
  return createProcess(commands, run_info);
}

