// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _JUDGE_CLIENT_TRACE_H__
#define _JUDGE_CLIENT_TRACE_H__

#include <string>

using namespace std;

class  TraceCallback {
  public :
    TraceCallback() :
      result_(-1),
      exited_(false) {
      TraceCallback::instance_ = this;
    }

    virtual ~TraceCallback() {
      TraceCallback::instance_ = NULL;
    }

    virtual bool onClone() {
      return false;
    }

    virtual void onError();
    virtual bool onExecve();
    virtual void onExit();
    virtual bool onOpen(const string& path, int flags);
    virtual void onMemoryLimitExceeded();
    virtual void onRestrictedFunction();

    void processResult(int status);
    void processSyscall(pid_t pid, int syscall);

    int getResult() const {
      return result_;
    }

    void setResult(int result) {
      result_ = result;
    }

    bool hasExited() {
      return exited_;
    }

    static TraceCallback* getInstance() {
      return TraceCallback::instance_;
    }

  protected :
    int result_;
    bool exited_;

  private :
    static TraceCallback* instance_;
};

#endif
