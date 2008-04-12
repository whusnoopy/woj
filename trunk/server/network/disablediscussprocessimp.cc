#include "disablediscussprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "util/calulate.h"
#include "object/discuss.h"
#include "object/list.h"
#include "object/info.h"
#include "data/datainterface.h"
using namespace std;

void DisableDiscussProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process Disable Disucss for:" << ip;
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
  Discuss discuss;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find discuss_id from data for:" << ip;
    return;
  }
  discuss.setMessageId(atoi(iter->c_str()));
  discuss = DataInterface::getInstance().getDiscuss(discuss.getMessageId());
  int ret =0;
  ret = DataInterface::getInstance().disableDiscuss(discuss);
  if (ret) {
    sendReply(socket_fd, 'N');
    return;
  }
  
  if (sendReply(socket_fd, 'Y')){
    LOG(ERROR) << "Cannot reply:" << ip;
    return;
  }
  LOG(INFO) << "Process Completed for:" << ip;
}

