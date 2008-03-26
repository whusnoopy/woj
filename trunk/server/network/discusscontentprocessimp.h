#ifndef _FLOOD_SERVER_NETWORK_DISCUSSCONTENTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_DISCUSSCONTENTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class DiscussContentProcessImp : public ProcessImp{
public:
  DiscussContentProcessImp() {}
  virtual ~DiscussContentProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

