#include "codeprocessimp.h"

#include <vector>
#include <string>

#include "util/calulate.h"
#include "object/user.h"
#include "object/code.h"
#include "base/logging.h"
#include "base/flags.h"
#include "base/util.h"
#include "data/datainterface.h"
using namespace std;

void CodeProcessImp::process(int socket_fd, const string& ip, int length) {
  LOG(INFO) << "Process the code for:" << ip;
  char* buf;
  buf = new char[length + 1];
  memset(buf, 0, length + 1);
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from" << ip;
    delete[] buf;
    return;
  }
  string data(buf, buf + length);
  delete[] buf;
  vector<string> datalist;
  spriteString(data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  if (iter != datalist.end()) {
    LOG(ERROR) << "Cannot find code_id from data for:" << ip;
    return;
  }
  int code_id = atoi(iter->c_str());
  iter++;
    /*do not need*/
    /*
    if (iter != datalist.end()) {
      LOG(ERROR) << "Cannot find user_id from data for:" << ip;
      return;
    }
    string user_id;
    user_id = *iter;*/
    /*do not need*/
  Code code;
  code = DataInterface::getInstance().getCode(code_id);
    /* do not need */
    /*bool can_read = false;
    User user;
    //user = DatabaseInterface::getInstance().getUserInfo(user_id);
    //can_read = DatabaseInterface::getInstance().IsCodeOwner(user_id, code_id);
    if (user.getPermission() & 0x01) {
      can_read = true;
    }
    if (!can_read) {
      sendReply(socket_fd, 'N');
      return;
    }
    if (sendReply(socket_fd, 'Y')) {
      LOG(ERROR) << "Cannot write data to:" << ip;
      return;
    }*/
    /* do not need*/
  string source = code.getCodeContent();
  string len = stringPrintf("%010d", source.length());
  if (socket_write(socket_fd, len.c_str(), 10)) {
    LOG(ERROR) << "Cannot write code length to:" << ip;
    return;
  }
  if (socket_write(socket_fd, source.c_str(), source.length())) {
    LOG(ERROR) << "Cannot write source to:" << ip;
    return;
  }
  LOG(INFO) << "Process code completed for:" << ip;
}

