#ifndef _FLOOD_SERVER_NETWORK_ABLECONTESTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ABLECONTESTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AbleContestProcessImp : public ProcessImp{
public:
  AbleContestProcessImp() {}
  virtual ~AbleContestProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

