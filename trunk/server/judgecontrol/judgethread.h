#ifndef _SERVER_JUDGECONTROL_JUDGE_THREAD_H__
#define _SERVER_JUDGECONTROL_JUDGE_THREAD_H__

#include "util/thread.h"
#include "object/list.h"
#include <iostream>
using namespace std;

class JudgeThread : public Thread {
public:
  JudgeThread() {
    flag = false;
  }
  virtual ~JudgeThread() {}

  void quit();
  void failed();
	void running();
  bool check(const string& ip);
private:
  bool flag;

  int sendFile(int connect_fd, const JudgeMission& mission, const string& ip);
};

#endif
