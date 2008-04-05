#ifndef _SERVER_JUDGECONTROL_SEMAPHORE_H__
#define _SERVER_JUDGECONTROL_SEMAPHORE_H__

#include <semaphore.h>

class Semaphore {
public:
  Semaphore();
  ~Semaphore();

  void p();
  void v();
private:
  sem_t sem_fd_;
};

#endif

