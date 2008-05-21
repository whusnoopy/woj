#ifndef _FLOOD_SERVER_NETWORK_TEACHERLISTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_TEACHERLISTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class TeacherListProcessImp : public ProcessImp{
public:
  TeacherListProcessImp() {}
  virtual ~TeacherListProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

