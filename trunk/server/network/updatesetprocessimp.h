#ifndef _FLOOD_SERVER_NETWORK_UPDATESETPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_UPDATESETPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class UpdateSetProcessImp : public ProcessImp{
public:
  UpdateSetProcessImp() {}
  virtual ~UpdateSetProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

