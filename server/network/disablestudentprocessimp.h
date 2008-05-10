#ifndef _FLOOD_SERVER_NETWORK_DISABLESTUDENTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_DISABLESTUDENTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class DisableStudentProcessImp : public ProcessImp{
public:
  DisableStudentProcessImp() {}
  virtual ~DisableStudentProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

