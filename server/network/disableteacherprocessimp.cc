#include "disableteacherprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "data/teachinterface.h"
#include "util/calulate.h"
#include "object/teacher.h"
#include "object/list.h"
#include "object/info.h"
using namespace std;

void DisableTeacherProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process disable Teacher for:" << ip;
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
    LOG(ERROR) << "Cannot find user_id from data for:" << ip;
    return;
  }
  string user_id = *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find available from data for:" << ip;
    return;
  }
  bool available = (*iter == "Y");
  iter++;
  int ret = TeachInterface::getInstance().disableTeacher(user_id, available);
  if (ret) {
    sendReply(socket_fd, 'N');
    LOG(ERROR) << "disable Teacher error";
    return;
  }
  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot reply to " << ip;
    return;
  }
  LOG(INFO) << "Process update teacher completed for:" << ip;
}

