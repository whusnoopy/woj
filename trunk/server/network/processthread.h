#ifndef _FLOOD_TEAM_PROCESS_THREAD_
#define _FLOOD_TEAM_PROCESS_THREAD_

#include "thread.h"
//#include "processimp.h"
#include <iostream>
using namespace std;

class ProcessThread : public Thread {
public:
  ProcessThread(int connect_socket) : m_socket(connect_socket){
	}
  virtual ~ProcessThread() {}

	void running();
private:
	int m_socket;
	//ProcessImp m_process_imp;
};

#endif
