#ifndef _SERVER_EXCPTION_SIGNALEXCEPTION
#define _SERVER_EXCPTION_SIGNALEXCEPTION


#include <exception>

#include <execinfo.h>
#include <signal.h>

#include "base/logging.h"
#include "exception/exceptiontracer.h"

using namespace std;

class SignalException: public ExceptionTracer, public exception {
  public:
    SignalException() {}

    static int getSignal();
};

#endif
