#include <string>
#include <set>

#include "clientthread.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "processimp.h"
#include "clientprocessimp.h"
#include "clientstatusprocessimp.h"
#include "clientloginprocessimp.h"
#include "clientsubmitprocessimp.h"
#include "base/logging.h"
#include "base/util.h"
#include "base/flags.h"
#include "util/calulate.h"
#include "object/configure.h"
using namespace std;

bool ClientThread::check(const string& ip) {
  return true;
}

void ClientThread::running(){
  //cout<<"Socket is "<<m_socket<<endl;
  int connect_fd;
  struct sockaddr_in childaddr;
  socklen_t len = sizeof(struct sockaddr);
  while(!flag){
    pthread_mutex_lock(m_lock);
    connect_fd = accept(m_socket, (struct sockaddr *)&childaddr, &len);
    pthread_mutex_unlock(m_lock);
    string ip = getIp(ntohl(childaddr.sin_addr.s_addr));
    if (!check(ip)) {
      LOG(WARNING) << "Unknown connection from:" << ip;
      close(connect_fd);
      continue;
    }
    LOG(INFO) << "Connection from :" << ip; 
    char buf[20];
    if (socket_read(connect_fd, buf, 10) != 10) {
      LOG(ERROR) << "header reader error";
      close(connect_fd);
      continue;
    }
    int type = (buf[0] - 'a') * 26 + buf[1] - 'a';
    bool unknown = false;
    switch (type) {
      case 294:
        m_process_imp = new ClientLoginProcessImp();
        break;
      case 487:
        m_process_imp = new ClientStatusProcessImp();
        break;
      case 67:  //cp
        m_process_imp = new ClientProcessImp();
        break;
      case 480:  //sm
        m_process_imp = new ClientSubmitProcessImp();
        break;
      default:
        LOG(ERROR) << "Unknown type data.";
        close(connect_fd);
        unknown = true;
        break;
    }
    if (unknown) 
      continue;
    int length = atoi(buf+2);
    m_process_imp->process(connect_fd, ip, length);
    delete m_process_imp;
    m_process_imp = NULL;
	  close(connect_fd);
  }
}

void ClientThread::quit(){
  flag = true;
}

