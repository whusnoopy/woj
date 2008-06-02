#ifndef _FLOOD_SERVER_NETWORK_STATUSLISTFORTEACHERPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_STAUTSLISTFORTEACHERPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class StatusListForTeacherProcessImp : public ProcessImp{
public:
  StatusListForTeacherProcessImp() {}
  virtual ~StatusListForTeacherProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

