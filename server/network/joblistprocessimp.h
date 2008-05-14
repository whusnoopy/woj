#ifndef _FLOOD_SERVER_NETWORK_JOBLISTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_JOBLISTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class JobListProcessImp : public ProcessImp{
public:
  JobListProcessImp() {}
  virtual ~JobListProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

