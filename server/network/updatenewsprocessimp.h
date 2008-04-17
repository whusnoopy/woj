#ifndef _FLOOD_SERVER_NETWORK_UPDATENEWSPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_UPDATENEWSPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class UpdateNewsProcessImp : public ProcessImp{
public:
  UpdateNewsProcessImp() {}
  virtual ~UpdateNewsProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

