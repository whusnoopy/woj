#ifndef _FLOOD_SERVER_NETWORK_LISTJOBDONEPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_LISTJOBDONEPROCESSIMP_H__

#include <string>
#include <vector>

#include "object/job.h"

#include "processimp.h"
using namespace std;

class ListJobDoneProcessImp : public ProcessImp{
public:
  ListJobDoneProcessImp() {}
  virtual ~ListJobDoneProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
  int checkJobDone(const string&, const Job&);
private:
};

#endif

