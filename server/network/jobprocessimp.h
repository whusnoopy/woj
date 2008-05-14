#ifndef _FLOOD_SERVER_NETWORK_JOBPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_JOBPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class JobProcessImp : public ProcessImp{
public:
  JobProcessImp() {}
  virtual ~JobProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

