#ifndef _FLOOD_SERVER_NETWORK_ADDCONTROLCLASSPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDCONTROLCLASSPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddControlClassProcessImp : public ProcessImp{
public:
  AddControlClassProcessImp() {}
  virtual ~AddControlClassProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

