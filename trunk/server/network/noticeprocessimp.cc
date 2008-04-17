#include "noticeprocessimp.h"

#include <string>
#include <vector>
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "object/contest.h"
#include "object/list.h"
#include "data/datainterface.h"
using namespace std;

void NoticeProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the homepage data for :" << ip;
  string data = DataInterface::getInstance().getNotice();
  string len = stringPrintf("%010d",data.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, data.c_str(), data.length())) {
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  LOG(INFO) << "Process the homepage data completed for:" << ip;
}

