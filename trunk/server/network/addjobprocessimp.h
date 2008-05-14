#ifndef _FLOOD_SERVER_NETWORK_ADDJOBPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDJOBPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddJobProcessImp : public ProcessImp{
public:
  AddJobProcessImp() {}
  virtual ~AddJobProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

