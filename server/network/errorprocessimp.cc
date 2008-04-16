#include "errorprocessimp.h"

#include <vector>
#include <string>

#include "util/calulate.h"
#include "object/user.h"
#include "object/error.h"
#include "base/logging.h"
#include "base/flags.h"
#include "base/util.h"
#include "data/datainterface.h"
using namespace std;

void ErrorProcessImp::process(int socket_fd, const string& ip, int length) {
  LOG(INFO) << "Process the error for:" << ip;
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
  LOG(DEBUG) << "error :" << data;
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find error_id from data for:" << ip;
    return;
  }
  int error_id = atoi(iter->c_str());
  iter++;
  Error error = DataInterface::getInstance().getError(error_id);
  string source = error.getContent();
  string len = stringPrintf("%010d", source.length());
  if (socket_write(socket_fd, len.c_str(), 10)) {
    LOG(ERROR) << "Cannot write error length to:" << ip;
    return;
  }
  if (socket_write(socket_fd, source.c_str(), source.length())) {
    LOG(ERROR) << "Cannot write source to:" << ip;
    return;
  }
  LOG(INFO) << "Process error completed for:" << ip;
}

