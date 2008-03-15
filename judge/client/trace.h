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
      time_(0),
      memory_(0) {
      TraceCallback::instance_ = this;
    }

    virtual ~TraceCallback() {
      TraceCallback::instance_ = NULL;
    }

    virtual bool onClone() {
      return false;
    }

    virtual bool onExecve();

    virtual bool onOpen(const string& path, int flags);

    virtual void onMemoryLimitExceeded();

    virtual void onExit(pid_t pid);

    virtual void onSigchld(pid_t pid);

    virtual void onError();

    int getResult() const {
      return result_;
    }

    void setResult(int result) {
      result_ = result;
    }

    int getTime() const {
      return time_;
    }

    int getMemory() const {
      return memory_;
    }

    int hasExited() const {
      return result_ >= 0 && result_ != RUNNING;
    }

    static TraceCallback* getInstance() {
      return TraceCallback::instance_;
    }

  protected :
    int result_;
    int time_;
    int memory_;

  private :
    static TraceCallback* instance_;
};

class ExecutiveCallback : public TraceCallback {
  public :
    virtual void onExit(pid_t pid);
};

void installHandlers();

#endif
