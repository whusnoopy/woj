#ifndef _FLOOD_SERVER_NETWORK_ISJOBDONEPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ISJOBDONEPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class IsJobDoneProcessImp : public ProcessImp{
public:
  IsJobDoneProcessImp() {}
  virtual ~IsJobDoneProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

