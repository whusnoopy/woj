#ifndef _FLOOD_SERVER_NETWORK_COURSELISTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_COURSELISTPROCESSIMP_H__

#include <string>
#include <vector>

#include "object/job.h"

#include "processimp.h"
using namespace std;

class CourseListProcessImp : public ProcessImp{
public:
  CourseListProcessImp() {}
  virtual ~CourseListProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

