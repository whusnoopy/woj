#ifndef _FLOOD_SERVER_NETWORK_ADDSERIESUSERPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDSERIESUSERPROCESSIMP_H__

#include "processimp.h"

class AddSeriesUserProcessImp : public ProcessImp{
public:
  AddSeriesUserProcessImp(){}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

