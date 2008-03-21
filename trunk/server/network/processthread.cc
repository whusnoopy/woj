#include <string>

#include "processthread.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "processimp.h"
#include "problemprocessimp.h"
#include "mailcontentprocessimp.h"
#include "maillistprocessimp.h"
#include "addmailprocessimp.h"
#include "registerprocessimp.h"
#include "homepageprocessimp.h"
#include "loginprocessimp.h"
#include "problemlistprocessimp.h"
#include "existuserprocessimp.h"
#include "codeprocessimp.h"
#include "disablemailprocessimp.h"
#include "base/logging.h"
#include "base/util.h"
#include "base/flags.h"
using namespace std;

string getIp(int ip_){
  int a, b, c, d;
  a = ip_ % 256;
  ip_ /= 256;
  b = ip_ % 256;
  ip_ /= 256;
  c = ip_ % 256;
  ip_ /= 256;
  d = ip_ % 256;
  return stringPrintf("%d.%d.%d.%d", d, c, b, a);
}

void ProcessThread::running(){
  //cout<<"Socket is "<<m_socket<<endl;
  int connect_fd;
  struct sockaddr_in childaddr;
  socklen_t len = sizeof(struct sockaddr);
  while(!flag){
    pthread_mutex_lock(m_lock);
    connect_fd = accept(m_socket, (struct sockaddr *)&childaddr, &len);
    pthread_mutex_unlock(m_lock);
    string ip = getIp(ntohl(childaddr.sin_addr.s_addr));
    LOG(INFO) << "Connection from :" << ip; 
    char buf[20];
    if (socket_read(connect_fd, buf, 10) != 10) {
      LOG(ERROR) << "header reader error";
      close(m_socket);
      continue;
    }
    int type = (buf[0] - 'a') * 26 + buf[1] - 'a';
    switch (type) {
      case 488:
        m_process_imp = new RegisterProcessImp();
        break;
      case 294:
        m_process_imp = new LoginProcessImp();
        break;
      case 112:
        m_process_imp = new ExistUserProcessImp();
        break;
      case 401:
        m_process_imp = new ProblemListProcessImp();
        break;
      case 197:
        m_process_imp = new HomePageProcessImp();
        break;
      case 470:
        m_process_imp = new CodeProcessImp();
        break;
      case 90:
        m_process_imp = new DisableMailProcessImp();
        break;
      case 12:
        m_process_imp = new AddMailProcessImp();
        break;
      case 323:
        m_process_imp = new MailListProcessImp();
        break;
      case 314:
        m_process_imp = new MailContentProcessImp();
        break;
      case 391:
        m_process_imp = new ProblemProcessImp();
        break;
      default:
        LOG(ERROR) << "Unknown type data.";
        close(m_socket);
        continue;
    }
    //sendReply(connect_fd, 'a');
    int length = atoi(buf+2);
    m_process_imp->process(connect_fd, ip, length);
    delete m_process_imp;
    m_process_imp = NULL;
	  close(m_socket);
  }
}

void ProcessThread::quit(){
  flag = true;
}

