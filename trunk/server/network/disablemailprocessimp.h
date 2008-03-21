#ifndef _FLOOD_SERVER_NETWORK_DISABLEMAILPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_DISABLEMAILPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class DisableMailProcessImp : public ProcessImp{
public:
  DisableMailProcessImp() {}
  virtual ~DisableMailProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

