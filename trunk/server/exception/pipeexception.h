#ifndef _SERVER_EXCPTION_PIPEEXCEPTION
#define _SERVER_EXCPTION_PIPEEXCEPTION

#include "exception/signalexception.h"

using namespace std;

class PipeException: public SignalException {
  public:
    PipeException() {}
    
    static int getSignal();
};

#endif
