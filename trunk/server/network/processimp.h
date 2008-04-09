#ifndef _FLOOD_SERVER_NETWORK_PROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_PROCESSIMP_H__

#include <string>

using namespace std;

class ProcessImp{
public:
  ProcessImp() {}
  virtual ~ProcessImp() {}
  virtual void process(int socket_fd, const string& ip, int length);
};

#endif
