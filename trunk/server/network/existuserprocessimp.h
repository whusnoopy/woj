#ifndef _FLOOD_SERVER_NETWORK_EXISTUSERPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_EXISTUSERPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ExistUserProcessImp : public ProcessImp{
public:
  ExistUserProcessImp() {}
  ~ExistUserProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

