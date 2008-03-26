#ifndef _FLOOD_SERVER_NETWORK_DISCUSSLISTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_DISCUSSLISTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class DiscussListProcessImp : public ProcessImp{
public:
  DiscussListProcessImp() {}
  virtual ~DiscussListProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

