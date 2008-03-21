#ifndef _FLOOD_SERVER_NETWORK_PROBLEMPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_PROBLEMPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ProblemProcessImp : public ProcessImp{
public:
  ProblemProcessImp() {}
  virtual ~ProblemProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

