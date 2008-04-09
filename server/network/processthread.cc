#include <string>
#include <set>

#include "processthread.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "processimp.h"
#include "ranklistprocessimp.h"
#include "contestlistprocessimp.h"
#include "contestcontentprocessimp.h"
#include "problemstatisticsprocessimp.h"
#include "statusprocessimp.h"
#include "problemprocessimp.h"
#include "mailcontentprocessimp.h"
#include "maillistprocessimp.h"
#include "addmailprocessimp.h"
#include "contestproblemprocessimp.h"
#include "registerprocessimp.h"
#include "homepageprocessimp.h"
#include "loginprocessimp.h"
#include "problemlistprocessimp.h"
#include "discusscontentprocessimp.h"
#include "existuserprocessimp.h"
#include "codeprocessimp.h"
#include "disablemailprocessimp.h"
#include "userinfoprocessimp.h"
#include "contestranklistprocessimp.h"
#include "conteststatisticsprocessimp.h"
#include "discusslistprocessimp.h"
#include "adddiscussprocessimp.h"
#include "disablediscussprocessimp.h"
#include "updateuserprocessimp.h"
#include "disableuserprocessimp.h"
#include "addnewsprocessimp.h"
#include "addproblemprocessimp.h"
#include "addinputandoutputfileprocessimp.h"
#include "addfiletoproblemprocessimp.h"
#include "updateproblemprocessimp.h"
#include "mostproblemprocessimp.h"
#include "problemversionprocessimp.h"
#include "ableproblemprocessimp.h"
#include "getproblemfileprocessimp.h"
#include "adminproblemlistprocessimp.h"
#include "addcontestprocessimp.h"
#include "admincontestlistprocessimp.h"
#include "ablecontestprocessimp.h"
#include "contestversionprocessimp.h"
#include "updatecontestprocessimp.h"
#include "addfiletocontestprocessimp.h"
#include "getcontestfileprocessimp.h"
#include "addproblemtocontestprocessimp.h"
#include "addusertocontestprocessimp.h"
#include "submitprocessimp.h"
#include "rejudgeprocessimp.h"
#include "base/logging.h"
#include "base/util.h"
#include "base/flags.h"
#include "util/calulate.h"
#include "object/configure.h"
using namespace std;

bool ProcessThread::check(const string& ip) {
  return Configure::getInstance().getNetWorkIpTabs().count(ip) > 0;
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
      case 448:  //rg
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
      case 487:
        m_process_imp = new StatusProcessImp();
        break;
      case 486:
        m_process_imp = new ProblemStatisticsProcessImp();
        break;
      case 63:  //cl
        m_process_imp = new ContestListProcessImp();
        break;
      case 54:  //cc
        m_process_imp = new ContestContentProcessImp();
        break;
      case 67:  //cp
        m_process_imp = new ContestProblemProcessImp();
        break;
      case 452: //rk
        m_process_imp = new RankListProcessImp();
        break;
      case 528: //ui
        m_process_imp = new UserInfoProcessImp();
        break;
      case 69:  //cr
        m_process_imp = new ContestRankListProcessImp();
        break;
      case 70:  //cs
        m_process_imp = new ContestStatisticsProcessImp();
        break;
      case 89:  //dl
        m_process_imp = new DiscussListProcessImp();
        break;
      case 80:  //dc
        m_process_imp = new DiscussContentProcessImp();
        break;
      case 3:   //ad
        m_process_imp = new AddDiscussProcessImp();
        break;
      case 81:  //dd
        m_process_imp = new DisableDiscussProcessImp();
        break;
      case 540: //uu
        m_process_imp = new UpdateUserProcessImp();
        break;
      case 98:  //du
        m_process_imp = new DisableUserProcessImp();
        break;
      case 13:  //an
        m_process_imp = new AddNewsProcessImp();
        break;
      case 15:  //ap
        m_process_imp = new AddProblemProcessImp();
        break;
      case 222: //io
        m_process_imp = new AddInputAndOutputProcessImp();
        break;
      case 145: //fp
        m_process_imp = new AddFileToProblemProcessImp();
        break;
      case 535: //up
        m_process_imp = new UpdateProblemProcessImp();
        break;
      case 327: //mp
        m_process_imp = new MostProblemProcessImp();
        break;
      case 411: //pv
        m_process_imp = new ProblemVersionProcessImp();
        break;
      case 41:  //bp
        m_process_imp = new AbleProblemProcessImp();
        break;
      case 171: //gp
        m_process_imp = new GetProblemFileProcessImp();
        break;
      case 457: //rp
        m_process_imp = new AdminProblemListProcessImp();
        break;
      case 2:   //ac
        m_process_imp = new AddContestProcessImp();
        break;
      case 444: //rc
        m_process_imp = new AdminContestListProcessImp();
        break;
      case 28:  //bc
        m_process_imp = new AbleContestProcessImp();
        break;
      case 73:  //cv
        m_process_imp = new ContestVersionProcessImp();
        break;
      case 522: //uc
        m_process_imp = new UpdateContestProcessImp();
        break;
      case 132: //fc
        m_process_imp = new AddFileToContestProcessImp();
        break;
      case 158: //gc
        m_process_imp = new GetContestFileProcessImp();
        break;
      case 392: //pc
        m_process_imp = new AddProblemToContestProcessImp();
        break;
      case 72:  //cu
        m_process_imp = new AddUserToContestProcessImp();
        break;
      case 480:  //sm
        m_process_imp = new SubmitProcessImp();
        break;
      case 451:  //rj
        m_process_imp = new RejudgeProcessImp();
        break;
      default:
        LOG(ERROR) << "Unknown type data.";
        close(connect_fd);
        unknown = true;
        break;
    }
    if (unknown) 
      continue;
    //sendReply(connect_fd, 'a');
    int length = atoi(buf+2);
    m_process_imp->process(connect_fd, ip, length);
    delete m_process_imp;
    m_process_imp = NULL;
	  close(connect_fd);
  }
}

void ProcessThread::quit(){
  flag = true;
}

