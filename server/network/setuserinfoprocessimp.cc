#include "setuserinfoprocessimp.h"

#include <string>
#include <vector>

#include "object/user.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void SetUserInfoProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the Update User for:" << ip;
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
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find email from data for:" << ip;
    return;
  }
  string email = *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find nickname from data for:" << ip;
    return;
  }
  string nickname = *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find school from data for:" << ip;
    return;
  }
  string school = *iter;
  iter++; 
  User user;
  user = DatabaseInterface::getInstance().getUserInfo(user_id);
  user.setEmail(email);
  user.setNickname(nickname);
  user.setSchool(school);
  int ret = 0;
  ret = DatabaseInterface::getInstance().updateUser(user);
  if (ret) {
    sendReply(socket_fd, 'N');
    return;
  }

  if (sendReply(socket_fd, 'Y')){
    LOG(ERROR) << "Cannot reply the set user info for:" << ip;
    return;
  } 
  LOG(INFO) << "Process set User Info completed for" << ip;
}

