#ifndef _FLOOD_SERVER_NETWORK_CONTESTSTATISTICSPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_CONTESTSTATISTICSPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ContestStatisticsProcessImp : public ProcessImp{
public:
  ContestStatisticsProcessImp() {}
  virtual ~ContestStatisticsProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

