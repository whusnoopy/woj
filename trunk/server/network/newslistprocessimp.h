#ifndef _FLOOD_SERVER_NETWORK_NEWSLISTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_NEWSLISTPROCESSIMP_H__

#include "processimp.h"

class NewsListProcessImp : public ProcessImp{
public:
  NewsListProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:

};

#endif
