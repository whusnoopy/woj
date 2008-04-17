#ifndef _FLOOD_SERVER_NETWORK_SETUSERINFOPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_SETUSERINFOPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class SetUserInfoProcessImp : public ProcessImp{
public:
  SetUserInfoProcessImp() {}
  virtual ~SetUserInfoProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

