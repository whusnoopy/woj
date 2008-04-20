#include "pipeexception.h"

#include <signal.h>

int PipeException::getSignal() {
  return SIGPIPE; 
}


