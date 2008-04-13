#include "userinfoprocessimp.h"

#include <string>
#include <set>
#include <vector>

#include "object/user.h"
#include "object/info.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void UserInfoProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process User Information for:" << ip;
  char* buf;
  buf = new char[length+1];
  memset(buf,0,sizeof(buf));
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string read_data(buf, buf + length);
  delete[] buf;
  vector<string> datalist;
  spriteString(read_data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id from data for:" << ip;
    return;
  }
  string user_id = *iter;
  User user;
  LOG(DEBUG) << "here is ok";
  user = DataInterface::getInstance().getUserInfo(user_id);
  LOG(DEBUG) << "here is ok";
  ProblemSet ac;
  ac = DataInterface::getInstance().getUserACProblem(user_id, true);
  LOG(DEBUG) << "here is ok";
  int rank = 0;
  rank = DataInterface::getInstance().getUserRank(user_id);
  string databuf;
  databuf += stringPrintf("%d\001%d\001%d\001%s\001%s",
                          rank,
                          user.getSolved(),
                          user.getSubmit(),
                          user.getSchool().c_str(),
                          user.getEmail().c_str());
  
  ProblemSet::iterator problem_iter = ac.begin();
  while (problem_iter != ac.end()) {
    databuf += stringPrintf("\001%d", *problem_iter);
    problem_iter++;
  }

  string len = stringPrintf("%010d",databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process User Info completed for" << ip;
}

