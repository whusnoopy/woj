#ifndef _FLOOD_SERVER_NETWORK_USERBASEPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_USERBASEPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class UserBaseProcessImp : public ProcessImp{
public:
  UserBaseProcessImp() {}
  virtual ~UserBaseProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

