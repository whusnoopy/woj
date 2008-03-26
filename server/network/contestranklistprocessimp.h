#ifndef _FLOOD_SERVER_NETWORK_CONTESTRANKLISTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_CONTESTRANKLISTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ContestRankListProcessImp : public ProcessImp{
public:
  ContestRankListProcessImp() {}
  virtual ~ContestRankListProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

