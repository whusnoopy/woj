#ifndef _FLOOD_SERVER_NETWORK_SUBMITDOCPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_SUBMITDOCPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class SubmitDocProcessImp : public ProcessImp{
public:
  SubmitDocProcessImp() {}
  virtual ~SubmitDocProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

