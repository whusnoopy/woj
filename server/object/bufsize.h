#ifndef _FLOOD_SERVER_OBJECT_BUFSIZE_H__
#define _FLOOD_SERVER_OBJECT_BUFSIZE_H__

#include <string>
#include <iostream>
using namespace std;

class BufSize{
public:
  BufSize() {buf_ = NULL; size_ = 0;}
  ~BufSize();
  int alloc(int size);
  char* getBuf() const;
  int getSize() const;
private:
  char* buf_;
  int size_;
};

#endif

