#ifndef _FLOOD_SERVER_NETWORK_ADDSERIESSTUDENTTOCOURSEPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDSERIESSTUDENTTOCOURSEPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddSeriesStudentToCourseProcessImp : public ProcessImp{
public:
  AddSeriesStudentToCourseProcessImp() {}
  virtual ~AddSeriesStudentToCourseProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif


