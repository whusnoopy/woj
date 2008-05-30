#ifndef _FLOOD_SERVER_NETWORK_ADDSTUDENTTOCOURSEPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDSTUDENTTOCOURSEPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddStudentToCourseProcessImp : public ProcessImp{
public:
  AddStudentToCourseProcessImp() {}
  virtual ~AddStudentToCourseProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif


