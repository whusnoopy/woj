#ifndef _FLOOD_SERVER_NETWORK_REGISTERPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_REGISTERPROCESSIMP_H__

#include "processimp.h"

class RegisterProcessImp : public ProcessImp{
public:
  RegisterProcessImp(){}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

