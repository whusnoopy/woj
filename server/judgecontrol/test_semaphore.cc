#include "semaphore.h"

#include <list>
#include <string>
#include <iostream>
using namespace std;

Semaphore sep;
list<int> num_queue;
pthread_mutex_t lock;

void* SendMessage(void* param) {
  int i = 0;
  while (1) {
    sleep(1);
    pthread_mutex_lock(&lock);
    num_queue.push_back(i++);
    num_queue.push_back(i++);
    pthread_mutex_unlock(&lock);
    sep.v();
  }
}

void* Read1Message(void* param) {
  while (1) {
    sep.p();
    pthread_mutex_lock(&lock);
    cout << "process 1: ";
    cout << *num_queue.begin() << endl ;
    num_queue.pop_front();
    pthread_mutex_unlock(&lock);
    sleep(2);
  }
}

void* Read2Message(void* param) {
  while (1) {
    sep.p();
    pthread_mutex_lock(&lock);
    cout << "process 2: ";
    cout << *num_queue.begin() << endl ;
    num_queue.pop_front();  
    pthread_mutex_unlock(&lock);
    sleep(3);
  }
}

int main() {
  pthread_t t1, t2, t3;
  pthread_mutex_init(&lock, NULL);
  pthread_create(&t1, NULL, SendMessage, NULL);
  pthread_create(&t2, NULL, Read1Message, NULL);
  pthread_create(&t3, NULL, Read2Message, NULL);
  pthread_join(t1, NULL);
  pthread_mutex_destroy(&lock);
}

