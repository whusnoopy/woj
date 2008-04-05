#ifndef _JUDGE_CLIENT_RESULT_H__
#define _JUDGE_CLIENT_RESULT_H__

#include "base/judge_result.h"

class JudgeResult {
  public :
    JudgeResult() :
      result_(ACCEPTED),
      time_(0),
      memory_(0) {
      JudgeResult::instance_ = this;
    }

    virtual ~JudgeResult() {
      JudgeResult::instance_ = 0;
    }

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

    static JudgeResult* getInstance() {
      return JudgeResult::instance_;
    }

  protected :
    int result_;
    int time_;
    int memory_;

  private :
    static JudgeResult* instance_;
};

#endif

