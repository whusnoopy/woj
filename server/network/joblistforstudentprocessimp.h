#ifndef _FLOOD_SERVER_NETWORK_JOBLISTFORSTUDENTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_JOBLISTFORSTUDENTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class JobListForStudentProcessImp : public ProcessImp{
public:
  JobListForStudentProcessImp() {}
  virtual ~JobListForStudentProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

