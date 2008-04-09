#ifndef _FLOOD_SERVER_NETWORK_CONTESTCONTENTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_CONTESTCONTENTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ContestContentProcessImp : public ProcessImp{
public:
  ContestContentProcessImp() {}
  virtual ~ContestContentProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

