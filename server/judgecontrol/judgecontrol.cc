#include "judgecontrol.h"

#include <string>

#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "object/configure.h"
#include "object/inc.h"
#include "base/judge_result.h"
#include "data/datainterface.h"

using namespace std;

JudgeControl* JudgeControl::instance = NULL;

string translateResult(int result) {
  switch(result) { 
    case ACCEPTED:
      return "Accepted";
      break;
    case PRESENTATION_ERROR:
      return "Presentation Error";
      break;
    case TIME_LIMIT_EXCEEDED:
      return "Time Limit Exceeded";
      break;
    case MEMORY_LIMIT_EXCEEDED:
      return "Memory Limit Exceeded";
      break;
    case WRONG_ANSWER:
      return "Wrong Answer";
      break;
    case OUTPUT_LIMIT_EXCEEDED:
      return "Output Limit Exceeded";
      break;
    case COMPILE_ERROR:
      return "Compile Error";
      break;
    case RUNTIME_ERROR_SIGSEGV:
      return "Runtime Error Sigsegv";
    case RUNTIME_ERROR_SIGFPE:
      return "Runtime Error Sigfpe";
    case RUNTIME_ERROR_SIGBUS:
      return "Runtime Error Sigbus";
    case RUNTIME_ERROR_SIGABRT:
      return "Runtime Error Sigabrt";
    case RUNTIME_ERROR_JAVA:
      return "Runtime Error Java";
      break;
  }
  return "System Error";
}

JudgeControl::JudgeControl() {
  pthread_mutex_init(&queue_lock, NULL); 
  pthread_mutex_init(&socket_lock, NULL);
  pthread_mutex_init(&set_lock, NULL);
}

JudgeControl::~JudgeControl() {
  pthread_mutex_destroy(&queue_lock);
  pthread_mutex_destroy(&socket_lock);
  pthread_mutex_destroy(&set_lock);
}

void JudgeControl::initJudge() {
  Configure configure = Configure::getInstance();
  max_client_ = configure.getJudgeControlMaxClient();
  if (max_client_ > MAXJUDGE) 
    max_client_ = MAXJUDGE;
  port_ = configure.getJudgeControlPort();
}

void JudgeControl::addMission(const JudgeMission& judge_mission) {
  pthread_mutex_lock(&queue_lock);
  judge_queue.push_back(judge_mission);
  pthread_mutex_unlock(&queue_lock);
  semaphore.v();
}

void JudgeControl::start(){
  //configure judgeserver;
  struct sockaddr_in server_addr;
  if ((listen_fd_ = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    LOG(SYS_ERROR) << "Cannot create judge socket.";
    exit(-1);
  }
  bzero(&server_addr, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  server_addr.sin_port = htons(port_);
  
  if (bind(listen_fd_, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
    LOG(SYS_ERROR) << "Cannot bind judge socket.";
    close(listen_fd_);
    exit(-1);
  }
  
  if(listen(listen_fd_, max_client_) == -1) {
    LOG(SYS_ERROR) << "Cannot listen judge socket";
    close(listen_fd_);
    exit(-1);
  }
  LOG(INFO) << "Begin service the judge......";
  for (int i = 0; i < max_client_; i++){
    judge_pool_[i] = JudgeThread();
    judge_pool_[i].start();
  }
  LOG(INFO) << "Judge Pool creat complete.";
}


void JudgeControl::addRejudgeItem(const Status& status) {
  pthread_mutex_lock(&set_lock);
  rejudge_set[status.getStatusId()] = status;
  pthread_mutex_unlock(&set_lock);
}

void JudgeControl::putReJudgeItem(const Status& status) {
  pthread_mutex_lock(&set_lock);
  if (rejudge_set.count(status.getStatusId())) {
    Status old_status = rejudge_set[status.getStatusId()];
    if (old_status.getResult() != status.getResult()) {
      Mail mail;
      mail.setTopicId(-1);
      mail.setToUser(status.getUserId());
      mail.setFromUser("System");
      mail.setTitle("System mails - Rejudge Remind");
      string content; 
      content += string("Rejudge Status[") + stringPrintf("%d", status.getStatusId()) + 
                 "] from [" + translateResult(old_status.getResult()) + "] To [" + 
                 translateResult(status.getResult()) + "] for User[" + 
                 status.getUserId() + "]";
      mail.setContent(content);
      mail.setRead(false);
      mail.setTime(getLocalTimeAsString("%Y-%m-%d %H:%M:%S"));
      mail.setReaderDel(false);
      mail.setWriterDel(false);
      DataInterface::getInstance().addMail(mail);

      mail.setToUser("snoopy");
      mail.setFromUser("System");
      mail.setTitle("System mails - Rejudge Remind");
      mail.setContent(content);
      mail.setTime(getLocalTimeAsString("%Y-%m-%d %H:%M:%S"));
      DataInterface::getInstance().addMail(mail);
    }
    rejudge_set.erase(status.getStatusId());
  }
  pthread_mutex_unlock(&set_lock);
}

void JudgeControl::join() {
  for (int i = 0; i < max_client_; i++) {
    judge_pool_[i].join();
  }
  close(listen_fd_);
}

JudgeMission JudgeControl::getMission() {
  JudgeMission mission;
  pthread_mutex_lock(&queue_lock);
  mission = *judge_queue.begin();
  judge_queue.pop_front();
  pthread_mutex_unlock(&queue_lock);
  return mission;
}


