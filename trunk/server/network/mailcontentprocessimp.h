#ifndef _FLOOD_SERVER_NETWORK_MAILCONTENTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_MAILCONTENTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class MailContentProcessImp : public ProcessImp{
public:
  MailContentProcessImp() {}
  virtual ~MailContentProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

