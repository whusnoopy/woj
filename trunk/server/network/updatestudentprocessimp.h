#ifndef _FLOOD_SERVER_NETWORK_UPDATESTUDENTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_UPDATESTUDENTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class UpdateStudentProcessImp : public ProcessImp{
public:
  UpdateStudentProcessImp() {}
  virtual ~UpdateStudentProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

