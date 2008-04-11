#ifndef _FLOOD_SERVER_NETWORK_MOSTCONTESTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_MOSTCONTESTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class MostContestProcessImp : public ProcessImp{
public:
  MostContestProcessImp() {}
  virtual ~MostContestProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

