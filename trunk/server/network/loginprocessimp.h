#ifndef _FLOOD_SERVER_NETWORK_LOGINPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_LOGINPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class LoginProcessImp : public ProcessImp{
public:
  LoginProcessImp() {}
  virtual ~LoginProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

