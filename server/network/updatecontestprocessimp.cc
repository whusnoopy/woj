#include "updatecontestprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "util/calulate.h"
#include "object/contest.h"
#include "object/list.h"
#include "object/info.h"
#include "object/user.h"
#include "data/datainterface.h"
using namespace std;

void UpdateContestProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process update Contest for:" << ip;
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
    LOG(ERROR) << "Cannot find contest_id from data for:" << ip;
    return;
  }
  contest.setContestId(atoi(iter->c_str()));
  contest = DataInterface::getInstance().getContest(contest.getContestId());
  iter++;
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
  int ret = 0;
  ret = DataInterface::getInstance().updateContest(contest);
  if (ret != 0) {
    sendReply(socket_fd, 'N');
    return;
  }

  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot return result to:" << ip;
    return;
  }

  LOG(ERROR) << "Process update Contest completed for:" << ip;
}

