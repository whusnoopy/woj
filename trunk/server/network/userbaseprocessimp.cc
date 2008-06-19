#include "userbaseprocessimp.h"

#include <string>
#include <set>
#include <vector>

#include "object/user.h"
#include "object/info.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void UserBaseProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process User Base Information for:" << ip;
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
  spriteString(read_data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id from data for:" << ip;
    return;
  }
  string user_id = *iter;
  User user;
  user = DataInterface::getInstance().getUserInfo(user_id);
  string databuf;
  databuf += stringPrintf("%s\001%s\001%s\001%s\001%s\001%s",
                          user.getId().c_str(),
                          user.getNickname().c_str(),
                          user.getShareCode() ? "Y" : "N",
                          user.getSchool().c_str(),
                          user.getEmail().c_str(),
                          user.getShowEmail() ? "Y" : "N");
  
  string len = stringPrintf("%010d",databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process User Base Info completed for" << ip;
}

