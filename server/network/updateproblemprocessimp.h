#ifndef _FLOOD_SERVER_NETWORK_UPDATEPROBLEMPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_UPDATEPROBLEMPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class UpdateProblemProcessImp : public ProcessImp{
public:
  UpdateProblemProcessImp() {}
  virtual ~UpdateProblemProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

