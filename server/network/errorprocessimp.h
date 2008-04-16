#ifndef _FLOOD_SERVER_NETWORK_ERRORPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ERRORPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ErrorProcessImp : public ProcessImp{
public:
  ErrorProcessImp() {}
  virtual ~ErrorProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

