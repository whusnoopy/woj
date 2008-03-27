#ifndef _FLOOD_SERVER_NETWORK_ABLEPROBLEMPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ABLEPROBLEMPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AbleProblemProcessImp : public ProcessImp{
public:
  AbleProblemProcessImp() {}
  virtual ~AbleProblemProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

