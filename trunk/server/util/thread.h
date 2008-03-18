#ifndef _FLOOD_TEAM_THREAD_
#define _FLOOD_TEAM_THREAD_

#include <pthread.h>
#include <iostream>
using namespace std;

class Thread{
public:
  Thread() {}
	/*Thread(int connect_socket):m_socket(connect_socket){
	}*/
	virtual ~Thread() {}
	static void * run(void * arg);
	void start();
	virtual void running();
	void join();
private:
  pthread_t m_pthread_id;
};

#endif
