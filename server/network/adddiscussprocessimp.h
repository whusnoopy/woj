#ifndef _FLOOD_SERVER_NETWORK_ADDDISCUSSPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDDISCUSSPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddDiscussProcessImp : public ProcessImp{
public:
  AddDiscussProcessImp() {}
  virtual ~AddDiscussProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

