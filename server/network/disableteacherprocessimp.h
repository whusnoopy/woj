#ifndef _FLOOD_SERVER_NETWORK_DISABLETEACHERPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_DISABLETEACHERPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class DisableTeacherProcessImp : public ProcessImp{
public:
  DisableTeacherProcessImp() {}
  virtual ~DisableTeacherProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

