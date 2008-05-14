#ifndef _FLOOD_SERVER_NETWORK_UPDATEJOBPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_UPDATEJOBPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class UpdateJobProcessImp : public ProcessImp{
public:
  UpdateJobProcessImp() {}
  virtual ~UpdateJobProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

