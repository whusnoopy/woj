#ifndef _FLOOD_SERVER_NETWORK_ADDMAILPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDMAILPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddMailProcessImp : public ProcessImp{
public:
  AddMailProcessImp() {}
  virtual ~AddMailProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

