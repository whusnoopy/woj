#ifndef _SERVER_EXCPTION_SIGNALTRANSLATOR
#define _SERVER_EXCPTION_SIGNALTRANSLATOR


#include <exception>

#include <execinfo.h>
#include <signal.h>

#include "base/logging.h"

using namespace std;

template <typename SignalExceptionClass>
class SignalTranslator {
  public:
    SignalTranslator() {
      signal(SignalExceptionClass::getSignal(), signalHandler);
    }
    static void signalHandler(int) {
      throw SignalExceptionClass();
    }
    static SignalTranslator* getInstance() {
      if (instance == NULL) 
        instance = new SignalTranslator();
      return instance;
    }
  private:
    static SignalTranslator* instance;
};

template <typename SignalExceptionClass>
SignalTranslator<SignalExceptionClass>* SignalTranslator<SignalExceptionClass>::instance = NULL;

#endif
