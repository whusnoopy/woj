#include "loginprocessimp.h"

#include <string>
#include <vector>

#include "../object/user.h"
#include "../util/calulate.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void LoginProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the Login for:" << ip;
  char* buf;
  buf = new char[length+1];
  memset(buf,0,sizeof(buf));
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string data(buf);
  delete[] buf;
  vector<string> datalist;
  string user_id, password, connect_ip;
  spriteString(data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find userid from data for:" << ip;
    return;
  }
  user_id = *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find password from data for:" << ip;
    return;
  }
  iter++;
  password = *iter;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find ip from data for:" << ip;
  }
  connect_ip = *iter;
  User user;
  //user = DatabaseInterface::getInstance().getUserInfo(user_id);
  if (password != user.getPassword()) {
    sendReply(socket_fd, 'N');
    return ;
  }
  user.setLastLoginIp(connect_ip);
  user.setLastLoginTime(getLocalTimeAsString("%Y-%m-%d %H:%M:%S"));
  string indentify_code = calIndentifyCode(user_id);
  user.setIndentifyCode(indentify_code);
  //DatabaseInterface::getInstance().updateUserInfo(user);
  if (sendReply(socket_fd, 'Y')){
    LOG(ERROR) << "Cannot reply the login for:" << ip;
    return;
  } 
  string len = stringPrintf("%010d",indentify_code.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, indentify_code.c_str(), indentify_code.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process Login completed for" << ip;
}

