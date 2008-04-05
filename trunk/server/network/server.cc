#include <string>
#include <iostream>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#include "processthread.h"
#include "processimp.h"
#include "registerprocessimp.h"
#include "base/logging.h"
#include "base/flags.h"
#include "base/util.h"
using namespace std;
#define SERVER_PORT 3030
#define BACKLOG 20

int main(int argc, char* argv[]){
  if (parseFlags(argc,argv)) {
    LOG(SYS_ERROR) << "Cannot parse flags!";
    return -1;
  }
  pthread_mutex_t lock;
  pthread_mutex_init(&lock, NULL);
  ProcessThread threads[250];
  int listen_fd;
  struct sockaddr_in server_addr;
  if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    LOG(SYS_ERROR) << "Cannot create socket";
    exit(-1);
  }
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(SERVER_PORT);

  if (bind(listen_fd, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1){
    LOG(SYS_ERROR) << "Cannot bind socket.";
    close(listen_fd);
    exit(-1);
  }
  if (listen(listen_fd, BACKLOG) == -1) {
    LOG(SYS_ERROR) << "Cannot listen the socket.";
    close(listen_fd);
    exit(-1);
  }
  LOG(INFO) << "Begin listen socket..........";
  for (int i = 0; i < 250; i++){
    threads[i] = ProcessThread(listen_fd, &lock);
    threads[i].start();
  }
  for (int i = 0; i < 250; i++){
    threads[i].join();
  }
  pthread_mutex_destroy(&lock);
  return 0;
}

