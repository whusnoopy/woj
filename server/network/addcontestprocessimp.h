#ifndef _FLOOD_SERVER_NETWORK_ADDCONTESTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDCONTESTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddContestProcessImp : public ProcessImp{
public:
  AddContestProcessImp() {}
  virtual ~AddContestProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

