#ifndef _FLOOD_SERVER_NETWORK_GETCONTESTFILEPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_GETCONTESTFILEPROCESSIMP_H__

#include <vector>
#include <string>

#include "processimp.h"
using namespace std;

class GetContestFileProcessImp : public ProcessImp {
public:
  GetContestFileProcessImp() {}
  virtual ~GetContestFileProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

