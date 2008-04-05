#include "semaphore.h"
/*
Semaphore::Semaphore(key_t* key, int val) {
  union semun sem;
  sem.val = val;
  if (key == NULL)
    sem_fd_ = semget(IPC_PRIVATE, 1, IPC_CREAT | 0666);
  else
    sem_fd_ = semget(*key, 1, IPC_CREAT | 0666);
  if (sem_fd_ == -1) {
    LOG(SYS_ERROR) << "Cannot create semaphore.";
    exit(-1);
  }
  semctl(sem_fd_, 0, SETVAL, sem);
}

Semaphore::~Semaphore() {
  union semun sem;
  sem.val = 0;
  semctl(sem_fd_, 0, IPC_RMID, sem);
}

void Semaphore::p() {
  struct sembuf sem_buf = {0, -1, SEM_UNDO};
  semop(sem_fd_, &sem_buf, 1);
}

void Semaphore::v() {
  struct sembuf sem_buf = {0, +1, SEM_UNDO};
  semop(sem_fd_, &sem_buf, 1);
}
*/

Semaphore::Semaphore() {
  sem_init(&sem_fd_, 0, 0);
}

Semaphore::~Semaphore() {
  sem_destroy(&sem_fd_);
}

void Semaphore::p() {
  sem_wait(&sem_fd_);
}

void Semaphore::v() {
  sem_post(&sem_fd_);
}


