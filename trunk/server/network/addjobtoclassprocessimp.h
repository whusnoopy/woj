#ifndef _FLOOD_SERVER_NETWORK_ADDJOBTOCLASSPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDJOBTOCLASSPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddJobToClassProcessImp : public ProcessImp{
public:
  AddJobToClassProcessImp() {}
  virtual ~AddJobToClassProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

