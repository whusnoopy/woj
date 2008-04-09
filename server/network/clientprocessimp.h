#ifndef _SERVER_NETWORK_CLIENTPROCESSIMP_H__
#define _SERVER_NETWORK_CLIENTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ClientProcessImp : public ProcessImp{
public:
  ClientProcessImp() {}
  virtual ~ClientProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

