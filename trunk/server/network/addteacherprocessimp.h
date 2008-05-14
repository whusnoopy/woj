#ifndef _FLOOD_SERVER_NETWORK_ADDTEACHERPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDTEACHERPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddTeacherProcessImp : public ProcessImp{
public:
  AddTeacherProcessImp() {}
  virtual ~AddTeacherProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

