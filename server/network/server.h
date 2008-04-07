#ifndef _SERVER_NETWORK_JUDGECONTROL_H__
#define _SERVER_NETWORK_JUDGECONTROL_H__

#include <vector>

#include <pthread.h>

#include "object/list.h"
#include "network/processthread.h"
using namespace std;

#define MAXCLIENT 150

class Server{
public:
  Server();
  ~Server();
  void initServer();
  void start();
  void join();

private:
  pthread_mutex_t socket_lock;
  ProcessThread threads[MAXCLIENT];
  int max_client_;
  int port_;
  int listen_fd_;
};

#endif

