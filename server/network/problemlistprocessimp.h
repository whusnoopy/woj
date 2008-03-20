#ifndef _FLOOD_SERVER_NETWORK_PROBLEMLISTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_PROBLEMLISTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ProblemListProcessImp : public ProcessImp{
public:
  ProblemListProcessImp() {}
  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

