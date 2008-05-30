#ifndef _FLOOD_SERVER_NETWORK_SETRESULTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_SETRESULTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class SetResultProcessImp : public ProcessImp{
public:
  SetResultProcessImp() {}
  virtual ~SetResultProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

