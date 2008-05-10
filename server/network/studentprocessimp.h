#ifndef _FLOOD_SERVER_NETWORK_STUDENTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_STUDENTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class StudentProcessImp : public ProcessImp{
public:
  StudentProcessImp() {}
  virtual ~StudentProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

