// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <string>

#include <errno.h>

#include "logging.h"
#include "util.h"

int socket_write(int communicate_socket,
                 const void* buffer,
                 size_t buffer_length) {
  const char* cs = (const char*) buffer;
  while (buffer_length > 0) {
    int sent_length = write(communicate_socket, cs, buffer_length);
    if (sent_length == -1) {
      if (errno == EINTR) {
        // continue;
      }
      LOG(SYS_ERROR) << "Fail to write to socket: " << communicate_socket;
    }
    cs += sent_length;
    buffer_length -= sent_length;
  }
  return 0;
}

string getLocalTimeAsString(const char* format) {
  time_t t = time(NULL);
  struct tm tm;
  localtime_r(&t, &tm);
  char buf[1024];
  strftime(buf, sizeof(buf), format, &tm);
  return buf;
}

