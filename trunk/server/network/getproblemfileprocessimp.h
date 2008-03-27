#ifndef _FLOOD_SERVER_NETWORK_GETPROBLEMFILEPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_GETPROBLEMFILEPROCESSIMP_H__

#include <vector>
#include <string>

#include "processimp.h"
using namespace std;

class GetProblemFileProcessImp : public ProcessImp {
public:
  GetProblemFileProcessImp() {}
  virtual ~GetProblemFileProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

