#ifndef _FLOOD_SERVER_NETWORK_UPDATEUSERPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_UPDATEUSERPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class UpdateUserProcessImp : public ProcessImp{
public:
  UpdateUserProcessImp() {}
  virtual ~UpdateUserProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

