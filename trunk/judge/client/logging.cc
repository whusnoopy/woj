// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include <errno.h>
#include <syslog.h>

#include "util.h"

#include "logging.h"

const string LOG_LEVEL_NAME[] = {
  "SYS_ERROR", "ERROR", "WARNING", "FATAL", "INFO", "DEBUG"
};

class LogFile {
  public :
    ~LogFile() {
    }

    void write(const string& log_message) {
      fprintf(stderr, "%s", log_message.c_str());
    }
} logFile;

Log::Log(const char* filename, int line_number, int level) {
  log_stream_ << getLocalTimeAsString("%Y-%m-%d %H:%M:%S")
              << " [" << LOG_LEVEL_NAME[level] << "] "
              << filename << ":" << line_number << ": ";
  if (level = SYS_ERROR) {
    log_stream_ << strerror(errno) << ". ";
  }
}

Log::~Log() {
  log_stream_ << endl;
  logFile.write(log_stream_.str());
}

