#ifndef _FLOOD_SERVER_NETWORK_STANDARDTESTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_STANDARDTESTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class StandardTestProcessImp : public ProcessImp{
public:
  StandardTestProcessImp() {}
  virtual ~StandardTestProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

