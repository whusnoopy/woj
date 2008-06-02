#include "setresultprocessimp.h"

#include <vector>
#include <string>

#include "object/objectinc.h"
#include "object/bufsize.h"
#include "util/calulate.h"
#include "data/teachinterface.h"
using namespace std;

void SetResultProcessImp::process(int socket_fd, const string& ip, int length) {
  LOG(INFO) << "Process the Set Result for:" << ip;
  char* buf;
  buf = new char[length+1];
  memset(buf,0,sizeof(buf));
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string data(buf, buf + length);
  delete[] buf;
  vector<string> datalist;
  spriteString(data, 1, datalist);
  LOG(DEBUG) << data;
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find status_id from data for:" << ip;
    return;
  }
  LOG(DEBUG) << *iter;
  int status_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find result from data for:" << ip;
    return;
  }
  int result = atoi(iter->c_str());
  iter++;  
  int ret = TeachInterface::getInstance().setStatusResult(status_id, result);
  string databuf = stringPrintf("%010d", status_id);
  if (socket_write(socket_fd, databuf.c_str(), 10)) {
    LOG(ERROR) << "Cannot return status id to " << ip;
    return;
  }
  if (ret) {
    sendReply(socket_fd, 'N');
    LOG(ERROR) << "set Status Result to class error";
    return;
  }
  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot reply to " << ip;
    return;
  }
  LOG(INFO) << "Set Status Result process completed." << ip;
}

