#ifndef _FLOOD_SERVER_NETWORK_CONTESTVERSIONPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_CONTESTVERSIONPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class ContestVersionProcessImp : public ProcessImp{
public:
  ContestVersionProcessImp() {}
  virtual ~ContestVersionProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

