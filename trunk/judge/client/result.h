// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _JUDGE_CLIENT_RESULT_H__
#define _JUDGE_CLIENT_RESULT_H__

#include "base/judge_result.h"

class JudgeResult {
  public :
    static JudgeResult* getInstance() {
      if (instance_ == 0)
        instance_ = new JudgeResult();
      return instance_;
    }

    virtual ~JudgeResult() {
      if (instance_)
        delete instance_;
      instance_ = 0;
    }
    
    void init();

    void updateResult(int result);
    void updateTime(int time);
    void updateMemory(int memory);
    void onTimeLimitExceeded(int time_limit);

    int getResult() {
      return result_;
    }

    int getTime() {
      return time_;
    }

    int getMemory() {
      return memory_;
    }

  protected :
    JudgeResult() {}

    int result_;
    int time_;
    int memory_;

  private :
    static JudgeResult* instance_;
};

#endif

