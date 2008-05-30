#ifndef _FLOOD_SERVER_NETWORK_TEACHERPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_TEACHERPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class TeacherProcessImp : public ProcessImp{
public:
  TeacherProcessImp() {}
  virtual ~TeacherProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

