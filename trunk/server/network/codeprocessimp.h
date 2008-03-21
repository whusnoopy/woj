#ifndef _FLOOD_SERVER_NETWORK_CODEPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_CODEPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class CodeProcessImp : public ProcessImp{
public:
  CodeProcessImp() {}
  virtual ~CodeProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

