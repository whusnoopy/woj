#include "server/util/thread.h"
void * Thread::run(void * arg){
	Thread * t = (Thread *)arg;
	t->running();
  return ((void *)0);
}
void Thread::start(){
	int err;
	err = pthread_create(&m_pthread_id,NULL,run,(void*)this);
	if (err != 0)
		cout<<"Create Error!"<<endl;
}

void Thread::running(){
	//cout<<"socket Connection:"<<m_socket<<endl
}

void Thread::join(){
	int err;
	err = pthread_join(m_pthread_id,NULL);
	if (err !=0 )
		cout<<"Ending error!"<<endl;
}

