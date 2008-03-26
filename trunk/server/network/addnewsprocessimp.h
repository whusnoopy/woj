#ifndef _FLOOD_SERVER_NETWORK_ADDNEWSPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDNEWSPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddNewsProcessImp : public ProcessImp{
public:
  AddNewsProcessImp() {}
  virtual ~AddNewsProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

