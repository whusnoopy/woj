#ifndef _FLOOD_SERVER_NETWORK_DISABLEDISCUSSPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_DISABLEDISCUSSPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class DisableDiscussProcessImp : public ProcessImp{
public:
  DisableDiscussProcessImp() {}
  virtual ~DisableDiscussProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

