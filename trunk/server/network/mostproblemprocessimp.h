#ifndef _FLOOD_SERVER_NETWORK_MOSTPROBLEMPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_MOSTPROBLEMPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class MostProblemProcessImp : public ProcessImp{
public:
  MostProblemProcessImp() {}
  virtual ~MostProblemProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

