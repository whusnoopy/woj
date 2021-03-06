#include "clientserver.h"

#include <string>
#include <iostream>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#include "base/logging.h"
#include "base/flags.h"
#include "base/util.h"
#include "object/configure.h"
using namespace std;

ClientServer::ClientServer() {
  pthread_mutex_init(&socket_lock, NULL); 
}

ClientServer::~ClientServer() {
  pthread_mutex_destroy(&socket_lock);
}

void ClientServer::initClientServer() {
  Configure configure = Configure::getInstance();
  max_client_ = configure.getClientServerMaxClient();
  if (max_client_ > MAX_CLIENTSERVER_CLIENT) 
    max_client_ = MAX_CLIENTSERVER_CLIENT;
  port_ = configure.getClientServerPort();
}

void ClientServer::start() {
  struct sockaddr_in server_addr;
  if ((listen_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    LOG(SYS_ERROR) << "Cannot create socket";
    exit(-1);
  }
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(port_);

  if (bind(listen_fd_, (struct sockaddr*) &server_addr, sizeof(server_addr)) == -1){
    LOG(SYS_ERROR) << "Cannot bind socket.";
    close(listen_fd_);
    exit(-1);
  }
  if (listen(listen_fd_, max_client_) == -1) {
    LOG(SYS_ERROR) << "Cannot listen the socket.";
    close(listen_fd_);
    exit(-1);
  }
  LOG(INFO) << "Begin listen socket..........";
  for (int i = 0; i < max_client_; i++){
    threads[i] = ClientThread(listen_fd_, &socket_lock);
    threads[i].start();
  }
  LOG(INFO) << "Process Thread Pool create complete.";
}

void ClientServer::join() {
  for (int i = 0; i < max_client_; i++){
    threads[i].join();
  }
  close(listen_fd_);
}

