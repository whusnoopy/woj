#include "bufsize.h"

BufSize::~BufSize() {
  if (buf_ != NULL) {
    delete[] buf_;
  }
}

int BufSize::alloc(int size){
  if (size_ < size) {
    delete[] buf_;
    buf_ = new char[size];
    size_ = size;
    memset(buf_, 0, size_);
  }else {
    memset(buf_, 0, size_);
  }
  return 1;
}

char* BufSize::getBuf() const {
  return buf_;
}

int BufSize::getSize() const {
  return size_;
}

