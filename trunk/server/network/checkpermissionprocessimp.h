#ifndef _FLOOD_SERVER_NETWORK_CHECKPERMISSIONPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_CHECKPERMISSIONPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class CheckPermissionProcessImp : public ProcessImp{
public:
  CheckPermissionProcessImp() {}
  virtual ~CheckPermissionProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

