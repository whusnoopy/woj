#include "mostcontestprocessimp.h"

#include <string>
#include <vector>

#include "object/contest.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void MostContestProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the Most Contest for:" << ip;
  char* buf;
  buf = new char[length+1];
  memset(buf,0,sizeof(buf));
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string read_data(buf);
  delete[] buf;
  vector<string> datalist;
  spriteString(read_data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find contest_id from data for:" << ip;
    return;
  }
  int contest_id = atoi(iter->c_str());
  Contest contest;
  contest = DataInterface::getInstance().getContest(contest_id);
  ProblemIdList problem_list = DataInterface::getInstance().getContestProblems(contest_id);
  string databuf;
  string len = stringPrintf("%010d", 0);
  if ((contest.getContestId() == 0)){
    socket_write(socket_fd, len.c_str(), 10);
    return;
  }
  databuf = stringPrintf("%d\001%s\001%s\001%s\001%s\001%s",
                          contest.getPublicId(),
                          contest.getTitle().c_str(),
                          contest.getDescription().c_str(),
                          contest.getStartTime().c_str(),
                          contest.getEndTime().c_str(),
                          contest.getType().c_str());

  ProblemIdList::iterator problem_iter = problem_list.begin();
  while (problem_iter != problem_list.end()) {
    databuf += stringPrintf("\001%d", *problem_iter);  
    problem_iter++;
  }

  len = stringPrintf("%010d",databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process Most Problem completed for" << ip;
}

