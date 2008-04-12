#include "addcontestprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "data/datainterface.h"
#include "util/calulate.h"
#include "object/contest.h"
#include "object/list.h"
#include "object/info.h"
#include "object/user.h"
using namespace std;

void AddContestProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process add Contest for:" << ip;
  char* buf;
  buf = new char[length + 1];
  memset(buf, 0, length + 1);
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
  Contest contest;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find title from data for:" << ip;
    return;
  }
  contest.setTitle(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find description from data for:" << ip;
    return;
  }
  contest.setDescription(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find start_time from data for:" << ip;
    return;
  }
  contest.setStartTime(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find end_time from data for:" << ip;
    return;
  }
  contest.setEndTime(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find type from data for:" << ip;
    return;
  }
  contest.setType(*iter);
  iter++;
  contest.setPublicId(0);
  contest.setVersion(1);
  contest.setAvailable(true);
  int contest_id = 0;
  contest_id = DataInterface::getInstance().addContest(contest);
  string databuf = stringPrintf("%010d", contest_id);
  
  LOG(DEBUG) << "Before Send contest_id";
  if (socket_write(socket_fd, databuf.c_str(), 10) != 0) {
    LOG(ERROR) << "Cannot return contest_id to:" << ip;
    return;
  }

  LOG(INFO) << "Process add Contest completed for:" << ip;
}

