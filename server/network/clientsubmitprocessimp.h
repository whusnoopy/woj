#ifndef _FLOOD_SERVER_NETWORK_CLIENTSUBMITPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_CLIENTSUBMITPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ClientSubmitProcessImp : public ProcessImp{
public:
  ClientSubmitProcessImp() {}
  virtual ~ClientSubmitProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

