#ifndef _FLOOD_SERVER_NETWORK_CONTESTPROBELMPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_CONTESTPROBLEMPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ContestProblemProcessImp : public ProcessImp{
public:
  ContestProblemProcessImp() {}
  virtual ~ContestProblemProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

