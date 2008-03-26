#ifndef _FLOOD_SERVER_NETWORK_RANKLISTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_RANKLISTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class RankListProcessImp : public ProcessImp{
public:
  RankListProcessImp() {}
  virtual ~RankListProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

