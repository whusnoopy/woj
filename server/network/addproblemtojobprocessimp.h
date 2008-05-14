#ifndef _FLOOD_SERVER_NETWORK_ADDPROBLEMTOJOBPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDPROBLEMTOJOBPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddProblemToJobProcessImp : public ProcessImp{
public:
  AddProblemToJobProcessImp() {}
  virtual ~AddProblemToJobProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

