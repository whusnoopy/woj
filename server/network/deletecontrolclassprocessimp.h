#ifndef _FLOOD_SERVER_NETWORK_DELETECONTROLCLASSPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_DELETECONTROLCLASSPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class DeleteControlClassProcessImp : public ProcessImp{
public:
  DeleteControlClassProcessImp() {}
  virtual ~DeleteControlClassProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

