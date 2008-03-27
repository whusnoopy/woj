#ifndef _FLOOD_SERVER_NETWORK_PROBLEMVERSIONPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_PROBLEMVERSIONPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ProblemVersionProcessImp : public ProcessImp{
public:
  ProblemVersionProcessImp() {}
  virtual ~ProblemVersionProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

