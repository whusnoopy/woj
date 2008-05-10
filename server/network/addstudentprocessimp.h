#ifndef _FLOOD_SERVER_NETWORK_ADDSTUDENTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDSTUDENTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddStudentProcessImp : public ProcessImp{
public:
  AddStudentProcessImp() {}
  virtual ~AddStudentProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

