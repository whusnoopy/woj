#ifndef _FLOOD_SERVER_NETWORK_HOMEPAGEPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_HOMEPAGEPROCESSIMP_H__

#include "processimp.h"

class HomePageProcessImp : public ProcessImp{
public:
  HomePageProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:

};

#endif
