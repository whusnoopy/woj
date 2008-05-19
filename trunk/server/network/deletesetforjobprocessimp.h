#ifndef _FLOOD_SERVER_NETWORK_DELETESETFORJOBPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_DELETESETFORJOBPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class DeleteSetForJobProcessImp : public ProcessImp{
public:
  DeleteSetForJobProcessImp() {}
  virtual ~DeleteSetForJobProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

