#include "userlistprocessimp.h"

#include <string>
#include <vector>

#include "object/info.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void UserListProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process User List for:" << ip;
  char* buf;
  buf = new char[length+1];
  memset(buf,0,sizeof(buf));
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string read_data(buf, buf + length);
  delete[] buf;
  vector<string> datalist;
  UserInfo user_info;
  spriteString(read_data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id from data for:" << ip;
    return;
  }
  user_info.user_id = (*iter == "?") ? "NULL" : *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find nickname from data for:" << ip;
    return;
  }
  user_info.nickname = (*iter == "?") ? "NULL" : *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find page_id from data for:" << ip;
    return;
  }
  user_info.page_id = atoi(iter->c_str());
  iter++;

  UserList user_list = DataInterface::getInstance().getUserList(user_info);
  UserList::iterator user_iter = user_list.begin();
  string databuf;
  bool first = true;
  while (user_iter != user_list.end()) {
    if (first) {
      databuf += stringPrintf("%s\001%s\001%s\001%s\001%s",
                              user_iter->user_id.c_str(),
                              user_iter->nickname.c_str(),
                              user_iter->last_login_ip.c_str(),
                              user_iter->last_login_time.c_str(),
                              user_iter->reg_time.c_str());
      first = false;
    }else {
      databuf += stringPrintf("\001%s\001%s\001%s\001%s\001%s",
                              user_iter->user_id.c_str(),
                              user_iter->nickname.c_str(),
                              user_iter->last_login_ip.c_str(),
                              user_iter->last_login_time.c_str(),
                              user_iter->reg_time.c_str());
    }
    user_iter++;
  }
  LOG(DEBUG) << databuf;
  string len = stringPrintf("%010d",databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process User List completed for" << ip;
}


