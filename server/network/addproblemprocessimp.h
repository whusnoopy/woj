#ifndef _FLOOD_SERVER_NETWORK_ADDPROBLEMPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDPROBLEMPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddProblemProcessImp : public ProcessImp{
public:
  AddProblemProcessImp() {}
  virtual ~AddProblemProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

