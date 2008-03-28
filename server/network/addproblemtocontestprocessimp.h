#ifndef _FLOOD_SERVER_NETWORK_ADDPROBLEMTOCONTESTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDPROBLEMTOCONTESTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddProblemToContestProcessImp : public ProcessImp{
public:
  AddProblemToContestProcessImp() {}
  virtual ~AddProblemToContestProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

