#ifndef _FLOOD_SERVER_NETWORK_STUDENTLISTPROCESSIMP_H__
#define _FLOOD_SERVER_NETWORK_STUDENTLISTPROCESSIMP_H__

#include <string>
#include <vector>

#include "processimp.h"
using namespace std;

class StudentListProcessImp : public ProcessImp{
public:
  StudentListProcessImp() {}
  virtual ~StudentListProcessImp() {}

  void process(int socket_fd, const string& ip, int length);
private:
};

#endif

