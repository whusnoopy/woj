#ifndef _FLOOD_SERVER_NETWORK_DELETESTUDENTFROMCOURSEPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_DELETESTUDENTFROMCOURSEPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class DeleteStudentFromCourseProcessImp : public ProcessImp{
public:
  DeleteStudentFromCourseProcessImp() {}
  virtual ~DeleteStudentFromCourseProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif


