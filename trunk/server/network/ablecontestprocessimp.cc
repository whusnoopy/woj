#include "ablecontestprocessimp.h"

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

void AbleContestProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process able contest for:" << ip;
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
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find available from data for:" << ip;
    return;
  }
  contest.setAvailable(*iter == "Y");
  int ret = 0;
  ret = DataInterface::getInstance().disableContest(contest);
  if (ret) {
    sendReply(socket_fd, 'N');
    return;
  }

  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot return problem_id to:" << ip;
    return;
  }

  LOG(ERROR) << "Process able contest completed for:" << ip;
}

