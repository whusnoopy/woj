#include "contestproblemprocessimp.h"

#include <string>
#include <vector>

#include "object/contest.h"
#include "object/info.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void ContestProblemProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the ContestList for:" << ip;
  char* buf;
  buf = new char[length+1];
  memset(buf,0,sizeof(buf));
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string data(buf, buf + length);
  delete[] buf;
  vector<string> datalist;
  spriteString(data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  string user_id;
  int contest_id;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find contest_id from data for:" << ip;
    return;
  }
  contest_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id form data for:" << ip;
    return;
  }
  user_id = *iter;
  string databuf;
  bool permission = false;
  permission = DataInterface::getInstance().checkPermission(contest_id, user_id);
  if (!permission) {
    sendReply(socket_fd, 'N');
    LOG(ERROR) << "Cannot access the file:" << ip;
    return;
  }
  ContestProblemList contest_problems;
  contest_problems = DataInterface::getInstance().getContestProblemList(contest_id);
  ContestProblemList::iterator problem_iter = contest_problems.begin();
  bool first = true;
  while (problem_iter != contest_problems.end()) {
    if (first) {
      first = false;
      databuf += stringPrintf("%d\001%d\001\%s\001%d\001%d",
                              problem_iter->problem_id,
                              problem_iter->in_contest_id,
                              problem_iter->title.c_str(),
                              problem_iter->accepted,
                              problem_iter->total);
    }else {
      databuf += stringPrintf("\001%d\001%d\001\%s\001%d\001%d",
                              problem_iter->problem_id,
                              problem_iter->in_contest_id,
                              problem_iter->title.c_str(),
                              problem_iter->accepted,
                              problem_iter->total);
    }
    problem_iter++;
  }
  sendReply(socket_fd, 'Y');
  string len = stringPrintf("%010d",databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process ContestList completed for" << ip;
}

