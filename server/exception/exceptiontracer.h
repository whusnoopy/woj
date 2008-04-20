#ifndef _SERVER_EXCPTION_EXCEPTIONTRACER
#define _SERVER_EXCPTION_EXCEPTIONTRACER


#include <exception>

#include <execinfo.h>
#include <signal.h>

#include "base/logging.h"

using namespace std;

class ExceptionTracer {
  public:
    ExceptionTracer();
};

#endif
