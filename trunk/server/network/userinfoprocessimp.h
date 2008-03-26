#ifndef _FLOOD_SERVER_NETWORK_USERINFOPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_USERINFOPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class UserInfoProcessImp : public ProcessImp{
public:
  UserInfoProcessImp() {}
  virtual ~UserInfoProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

