#ifndef _FLOOD_SERVER_NETWORK_ADMINPROBLEMLISTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_ADMINPROBLEMLISTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class AdminProblemListProcessImp : public ProcessImp{
public:
  AdminProblemListProcessImp() {}
  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

