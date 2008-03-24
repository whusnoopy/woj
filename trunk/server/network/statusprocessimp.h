#ifndef _FLOOD_SERVER_NETWORK_STATUSPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_STAUTSPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class StatusProcessImp : public ProcessImp{
public:
  StatusProcessImp() {}
  virtual ~StatusProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

