#ifndef _FLOOD_SERVER_NETWORK_CONTESTLISTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_CONTESTLISTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ContestListProcessImp : public ProcessImp{
public:
  ContestListProcessImp() {}
  virtual ~ContestListProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

