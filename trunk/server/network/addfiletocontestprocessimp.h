#ifndef _FLOOD_SERVER_NETWORK_ADDFILETOCONTESTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDFILETOCONTESTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddFileToContestProcessImp : public ProcessImp{
public:
  AddFileToContestProcessImp() {}
  virtual ~AddFileToContestProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

