#ifndef _FLOOD_SERVER_NETWORK_SETNOTICEPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_SETNOTICEPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class SetNoticeProcessImp : public ProcessImp{
public:
  SetNoticeProcessImp() {}
  virtual ~SetNoticeProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

