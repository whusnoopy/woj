#include "clientprocessimp.h"

#include <vector>
#include <string>

#include "util/calulate.h"
#include "object/list.h"
#include "object/contest.h"
#include "object/problem.h"
#include "base/logging.h"
#include "base/flags.h"
#include "base/util.h"
#include "data/datainterface.h"
using namespace std;

void ClientProcessImp::process(int socket_fd, const string& ip, int length) {
  LOG(INFO) << "Process the Client for:" << ip;
  string databuf;
  ProblemIdList problem_list = DataInterface::getInstance().getClientProblemList();
  ContestInfoList contest_list = DataInterface::getInstance().getClientContestList();
  databuf = stringPrintf("%d", problem_list.size());
  ProblemIdList::iterator problem_iter = problem_list.begin();
  while (problem_iter != problem_list.end()) {
    databuf += stringPrintf("\001%d", *problem_iter); 
    problem_iter++;
  }


  databuf += stringPrintf("\001%d", contest_list.size());
  ContestInfoList::iterator contest_iter = contest_list.begin();
  while (contest_iter != contest_list.end()) {
    databuf += stringPrintf("\001%d\001%s\001%d", 
                            contest_iter->contest_id,
                            contest_iter->title.c_str(),
                            contest_iter->problem_list.size());
    ProblemIdList::iterator contest_problem_iter = contest_iter->problem_list.begin();
    LOG(DEBUG) << stringPrintf("problem_list size : %d", contest_iter->problem_list.size());
    while (contest_problem_iter != contest_iter->problem_list.end()) {
      databuf += stringPrintf("\001%d", *contest_problem_iter);
      contest_problem_iter++;
    }
    contest_iter++;
  }


  databuf += "\n";
  LOG(DEBUG) << databuf;

  string len = stringPrintf("%010d\n", databuf.length());
  if (socket_write(socket_fd, len.c_str(), 11)) {
    LOG(ERROR) << "Cannot write data length to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot write source to:" << ip;
    return;
  }
  LOG(INFO) << "Process Client completed for:" << ip;
}

