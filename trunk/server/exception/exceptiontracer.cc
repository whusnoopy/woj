#include "exceptiontracer.h"

ExceptionTracer::ExceptionTracer() {
  void * array[25];
  int size = backtrace(array, 25);
  char ** symbols = backtrace_symbols(array, size);

  for (int i = 0; i < size; i++) {
    LOG(ERROR) << symbols[i];
  }
  free(symbols);
}
