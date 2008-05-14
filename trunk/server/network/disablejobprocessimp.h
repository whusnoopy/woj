#ifndef _FLOOD_SERVER_NETWORK_DISABLEJOBPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_DISABLEJOBPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class DisableJobProcessImp : public ProcessImp{
public:
  DisableJobProcessImp() {}
  virtual ~DisableJobProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

