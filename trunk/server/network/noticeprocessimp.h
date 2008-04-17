#ifndef _FLOOD_SERVER_NETWORK_NOTICEPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_NOTICEPROCESSIMP_H__

#include "processimp.h"

class NoticeProcessImp : public ProcessImp{
public:
  NoticeProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:

};

#endif
