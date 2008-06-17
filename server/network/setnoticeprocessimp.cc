#include "setnoticeprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "data/datainterface.h"
#include "../util/calulate.h"
#include "../object/news.h"
#include "../object/list.h"
#include "../object/info.h"
#include "../object/user.h"
using namespace std;

void SetNoticeProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process set Notice for:" << ip;
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
  string time = getLocalTimeAsString("%Y-%m-%d %H:%M:%S");
  LOG(INFO) << read_data;
  int ret = 0;
  ret = DataInterface::getInstance().updateNotice(read_data, time);
  if (ret < 0) {
    sendReply(socket_fd, 'N');
    return;
  }
  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot reply to:" << ip;
    return;
  }
  LOG(ERROR) << "Process set Notice completed for:" << ip;
}

