#ifndef _FLOOD_SERVER_NETWORK_COURSEPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_COURSEPROCESSIMP_H__

#include <string>
#include <vector>

#include "object/job.h"

#include "processimp.h"
using namespace std;

class CourseProcessImp : public ProcessImp{
public:
  CourseProcessImp() {}
  virtual ~CourseProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

