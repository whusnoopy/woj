#ifndef _FLOOD_SERVER_NETWORK_DISABLEUSERPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_DISABLEUSERPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class DisableUserProcessImp : public ProcessImp{
public:
  DisableUserProcessImp() {}
  virtual ~DisableUserProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

