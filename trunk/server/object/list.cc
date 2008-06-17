#include "object/list.h"

bool operator<(const ProblemUserAC& a, const ProblemUserAC& b) {
  if (a.problem_id != b.problem_id) {
    return a.problem_id < b.problem_id;
  }
  return a.user_id < b.user_id;
}

bool operator<(const ContestRankListItem& a, const ContestRankListItem& b) {
  if (a.accepted != b.accepted) 
    return a.accepted > b.accepted;
  return a.penalty < b.penalty;
}

