#ifndef _FLOOD_SERVER_NETWORK_CHECKSTUDENTORTEACHERPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_CHECKSTUDENTORTEACHERPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class CheckStudentOrTeacherProcessImp : public ProcessImp{
public:
  CheckStudentOrTeacherProcessImp() {}
  virtual ~CheckStudentOrTeacherProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

