#include "list.h"

bool operator<(const ProblemUserAc& a, const ProblemUserAc& b) {
  if (a.problem_id != b.problem_id) {
    return a.problem_id < b.problem_id;
  }
  return a.user_id < b.user_id;
}

