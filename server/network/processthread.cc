#include <string>
#include <set>

#include "processthread.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

#include "processheader.h"
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
    LOG(DEBUG) << "Connection happen";
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
    LOG(DEBUG) << buf;
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
      case 210:  //ic
        m_process_imp = new StandardTestProcessImp();
        break;
      case 106:  //ec
        m_process_imp = new ErrorProcessImp();
        break;
      case 531:  //ul
        m_process_imp = new UserListProcessImp();
        break;
      case 461:  //rt
        m_process_imp = new CheckPermissionProcessImp();
        break;
      case 533:  //un
        m_process_imp = new UpdateNewsProcessImp();
        break;
      case 481:  //sn
        m_process_imp = new SetNoticeProcessImp();
        break;
      case 353:  //np
        m_process_imp = new NoticeProcessImp();
        break;
      case 488:  //su
        m_process_imp = new AddSeriesUserProcessImp();
        break;
      case 476:  //si
        m_process_imp = new SetUserInfoProcessImp();
        break;
      default: 
        unknown = processTeach(type);
        if (unknown) 
        LOG(ERROR) << "Unknown type data.";
        close(connect_fd);
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

bool ProcessThread::processTeach(int type) {
  switch (type) {
    case 18:   //as
      m_process_imp = new AddStudentProcessImp();
      return false;
    case 538:  //us
      m_process_imp = new UpdateStudentProcessImp();
      return false;
    case 96:   //ds
      m_process_imp = new DisableStudentProcessImp();
      return false;
    case 483:  //sp
      m_process_imp = new StudentProcessImp();
      return false;
    case 19:   //at
      m_process_imp = new AddTeacherProcessImp();
      return false;
    case 539:  //ut
      m_process_imp = new UpdateTeacherProcessImp();
      return false;
    case 97:   //dt
      m_process_imp = new DisableTeacherProcessImp();
      return false;
    case 0:    //aa
      m_process_imp = new AddControlClassProcessImp();
      return false;
    case 78:   //da
      m_process_imp = new DeleteControlClassProcessImp();
      return false;
    case 9:    //aj
      m_process_imp = new AddJobProcessImp();
      return false;
    case 399:  //pj
      m_process_imp = new AddProblemToJobProcessImp();
      return false;
    case 87:   //dj
      m_process_imp = new DisableJobProcessImp();
      return false;
    case 236:  //jc
      m_process_imp = new AddJobToClassProcessImp();
      return false;
    case 524:  //ue
      m_process_imp = new UpdateSetProcessImp();
      return false;
    case 477:  //sj
      m_process_imp = new DeleteSetForJobProcessImp();
      return false;
    case 217:  //ij
      m_process_imp = new IsJobDoneProcessImp();
      return false;
    case 245:  //jl
      m_process_imp = new JobListProcessImp();
      return false;
    case 249:  //jp
      m_process_imp = new JobProcessImp();
      return false;
    case 295:  //lj
      m_process_imp = new ListJobDoneProcessImp();
      return false;
    case 529:  //uj
      m_process_imp = new UpdateJobProcessImp();
      return false;
    case 122:  //es
      m_process_imp = new AddSeriesStudentProcessImp();
      return false;
    default:
      return true;
  }
  return true;
}

