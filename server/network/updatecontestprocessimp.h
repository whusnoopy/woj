#ifndef _FLOOD_SERVER_NETWORK_UPDATECONTESTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_UPDATECONTESTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class UpdateContestProcessImp : public ProcessImp{
public:
  UpdateContestProcessImp() {}
  virtual ~UpdateContestProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

