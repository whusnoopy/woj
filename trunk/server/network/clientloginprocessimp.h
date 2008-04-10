#ifndef _FLOOD_SERVER_NETWORK_CLIENTLOGINPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_CLIENTLOGINPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ClientLoginProcessImp : public ProcessImp{
public:
  ClientLoginProcessImp() {}
  virtual ~ClientLoginProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

