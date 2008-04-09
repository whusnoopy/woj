#ifndef _FLOOD_TEAM_CLIENT_THREAD_
#define _FLOOD_TEAM_CLIENT_THREAD_

#include "util/thread.h"
#include "processimp.h"
#include <iostream>
using namespace std;

class ClientThread : public Thread {
public:
  ClientThread(){}
  ClientThread(int listen_fd,  pthread_mutex_t* lock) : m_socket(listen_fd), m_lock(lock){
	  flag = false;
  }
  virtual ~ClientThread() {}

  void quit();

	void running();
  bool check(const string& ip);
private:
	int m_socket;
	ProcessImp* m_process_imp;
  pthread_mutex_t* m_lock;
  bool flag;
};

#endif
