// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _BASE_LOGGING_H__
#define _BASE_LOGGING_H__

#include <string>
#include <sstream>

using namespace std;

#define LOG(level) Log(__FILE__, __LINE__, level).stream()

enum LOG_LEVEL {
  SYS_ERROR = 0, ERROR, WARNING, FATAL, INFO, DEBUG
};

class Log {
  public :
    Log(const char* filename, int line_number, int level);
    ~Log();

    ostream& stream() { return log_stream_; }

  private :
    ostringstream log_stream_;
};

#endif

