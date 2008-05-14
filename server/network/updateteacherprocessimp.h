#ifndef _FLOOD_SERVER_NETWORK_UPDATETEACHERPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_UPDATETEACHERPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class UpdateTeacherProcessImp : public ProcessImp{
public:
  UpdateTeacherProcessImp() {}
  virtual ~UpdateTeacherProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

