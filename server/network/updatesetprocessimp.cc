#include "updatesetprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "data/teachinterface.h"
#include "util/calulate.h"
#include "object/job.h"
#include "object/list.h"
#include "object/info.h"
using namespace std;

void UpdateSetProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process Update Set for:" << ip;
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
  SetItem set_item;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find set_id from data for:" << ip;
    return;
  }
  set_item.set_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find should do number from data for:" << ip;
    return;
  }
  set_item.number = atoi(iter->c_str());
  iter++;
  while (iter != datalist.end()) {
    set_item.problem_list.push_back(aoti(iter->c_str()));
  }
  int ret = TeacherInterface::getInstance().updateSet(set_item);
  if (ret) {
    sendReply(socket_fd, 'N');
    LOG(ERROR) << "Update Set error";
    return;
  }
  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot reply to " << ip;
    return;
  }
  LOG(INFO) << "Process Update Set completed for:" << ip;
}

