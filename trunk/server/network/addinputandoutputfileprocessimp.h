#ifndef _FLOOD_SERVER_NETWORK_ADDINPUTANDOUTPUTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADDINPUTANDOUTPUTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AddInputAndOutputProcessImp : public ProcessImp{
public:
  AddInputAndOutputProcessImp() {}
  virtual ~AddInputAndOutputProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

