// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <signal.h>
#include <sys/wait.h>

#include "base/util.h"

#include "judge/util.h"

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

