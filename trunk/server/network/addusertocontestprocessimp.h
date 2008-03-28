#ifndef _FLOOD_SERVER_NETWORK_ADDUSERTOCONTESTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDUSERTOCONTESTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddUserToContestProcessImp : public ProcessImp{
public:
  AddUserToContestProcessImp() {}
  virtual ~AddUserToContestProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

