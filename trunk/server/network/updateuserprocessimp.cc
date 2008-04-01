#include "updateuserprocessimp.h"

#include <string>
#include <vector>

#include "object/user.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void UpdateUserProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the Update User for:" << ip;
  char* buf;
  buf = new char[length+1];
  memset(buf,0,sizeof(buf));
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string read_data(buf);
  delete[] buf;
  vector<string> datalist;
  spriteString(read_data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  User user_buf;
  string old_password;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id from data for:" << ip;
    return;
  }
  user_buf.setId(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find old_password from data for:" << ip;
    return;
  }
  old_password = *iter; 
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find new password from data for:" << ip;
    return;
  }
  user_buf.setPassword(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find email from data for:" << ip;
    return;
  }
  user_buf.setEmail(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find show_email from data for:" << ip;
    return;
  }
  user_buf.setShowEmail((*iter == "Y"));
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find nickname from data for:" << ip;
    return;
  }
  user_buf.setNickname(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find school from data for:" << ip;
    return;
  }
  user_buf.setSchool(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find share_code from data for:" << ip;
    return;
  }
  user_buf.setShareCode(*iter=="Y");
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find language from data for:" << ip;
    return;
  }
  user_buf.setLanguage(atoi(iter->c_str()));
  iter++;
 
  User user;
  user = DatabaseInterface::getInstance().getUserInfo(user_buf.getId());
  if (old_password != user.getPassword()) {
    sendReply(socket_fd, 'N');
    return ;
  }

  user.updateUser(user_buf);
  int ret = 0;
  ret = DatabaseInterface::getInstance().updateUser(user);
  if (ret) {
    sendReply(socket_fd, 'N');
    return;
  }

  if (sendReply(socket_fd, 'Y')){
    LOG(ERROR) << "Cannot reply the update user for:" << ip;
    return;
  } 
  LOG(INFO) << "Process Update User completed for" << ip;
}

