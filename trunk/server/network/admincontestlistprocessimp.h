#ifndef _FLOOD_SERVER_NETWORK_ADMINCONTESTLISTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADMINCONTESTLISTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AdminContestListProcessImp : public ProcessImp{
public:
  AdminContestListProcessImp() {}
  virtual ~AdminContestListProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

