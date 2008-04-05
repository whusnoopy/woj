#ifndef _FLOOD_SERVER_NETWORK_SUBMITPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_SUBMITPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class SubmitProcessImp : public ProcessImp{
public:
  SubmitProcessImp() {}
  virtual ~SubmitProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

