#include "judgecontrol.h"

#include <string>

#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "object/configure.h"
#include "object/inc.h"

using namespace std;

JudgeControl* JudgeControl::instance = NULL;

JudgeControl::JudgeControl() {
  pthread_mutex_init(&queue_lock, NULL); 
  pthread_mutex_init(&socket_lock, NULL);
}

JudgeControl::~JudgeControl() {
  pthread_mutex_destroy(&queue_lock);
  pthread_mutex_destroy(&socket_lock);
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


