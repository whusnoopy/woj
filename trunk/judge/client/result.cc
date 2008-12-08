// Copyright 2008 Flood Team of Wuhan Univ.
// Author: yewen@mail.whu.edu.cn (Wen, YE)

#include "judge/client/result.h"

JudgeResult* JudgeResult::instance_ = 0;

void JudgeResult::init() {
  result_ = ACCEPTED;
  time_ = 0;
  memory_ = 0;
}

void JudgeResult::updateResult(int result) {
  result_ = result;
}

void JudgeResult::onTimeLimitExceeded(int time_limit) {
  result_ = TIME_LIMIT_EXCEEDED;
  time_ = time_limit + 36;
}

void JudgeResult::updateTime(int time) {
  time_ += time;
}

void JudgeResult::updateMemory(int memory) {
  if (memory > memory_)
    memory_ = memory;
}
 
