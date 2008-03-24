#ifndef _FLOOD_SERVER_NETWORK_PROBLEMSTATISTICSPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_PROBLEMSTATISTICSPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ProblemStatisticsProcessImp : public ProcessImp{
public:
  ProblemStatisticsProcessImp() {}
  virtual ~ProblemStatisticsProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

