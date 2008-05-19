#ifndef _FLOOD_SERVER_NETWORK_ADDSERIESSTUDENTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDSERIESSTUDENTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddSeriesStudentProcessImp : public ProcessImp{
public:
  AddSeriesStudentProcessImp() {}
  virtual ~AddSeriesStudentProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

