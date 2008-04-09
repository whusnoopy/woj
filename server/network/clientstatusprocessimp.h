#ifndef _SERVER_NETWORK_CLIENTSTATUSPROCESSIMP_H__
#define _SERVER_NETWORK_CLIENTSTATUSPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ClientStatusProcessImp : public ProcessImp{
public:
  ClientStatusProcessImp() {}
  virtual ~ClientStatusProcessImp() {}

  void process(int socket_fd, const string& ip, int user_id);
private:
};

#endif

