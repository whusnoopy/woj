#include "disablemailprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "data/datainterface.h"
#include "../util/calulate.h"
#include "../object/mail.h"
#include "../object/list.h"
#include "../object/info.h"
using namespace std;

void DisableMailProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process DisableMail for:" << ip;
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
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find mail_id from data for:" << ip;
    return;
  }
  int mail_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id from data for:" << ip;
    return;
  }
  string user_id = *iter;
  int ret = DataInterface::getInstance().disableMail(user_id, mail_id);
  if (!ret) {
    sendReply(socket_fd, 'N');
    return;
  }
  if (sendReply(socket_fd, 'Y')){
    LOG(ERROR) << "Cannot reply:" << ip;
    return;
  }
  LOG(INFO) << "Process Completed for:" << ip;
}

