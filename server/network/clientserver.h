#ifndef _SERVER_NETWORK_CLIENTSERVER_H__
#define _SERVER_NETWORK_CLIENTSERVER_H__

#include <vector>

#include <pthread.h>

#include "object/list.h"
#include "network/clientthread.h"
using namespace std;

#define MAX_CLIENTSERVER_CLIENT 50

class ClientServer{
public:
  ClientServer();
  ~ClientServer();
  void initClientServer();
  void start();
  void join();

private:
  pthread_mutex_t socket_lock;
  ClientThread threads[MAX_CLIENTSERVER_CLIENT];
  int max_client_;
  int port_;
  int listen_fd_;
};

#endif

