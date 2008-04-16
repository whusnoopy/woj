#ifndef _FLOOD_SERVER_NETWORK_USERLISTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_USERLISTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class UserListProcessImp : public ProcessImp{
public:
  UserListProcessImp() {}
  virtual ~UserListProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

