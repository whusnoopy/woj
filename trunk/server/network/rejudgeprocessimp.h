#ifndef _FLOOD_SERVER_NETWORK_REJUDGEPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_REJUDGEPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class RejudgeProcessImp : public ProcessImp{
public:
  RejudgeProcessImp() {}
  virtual ~RejudgeProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

