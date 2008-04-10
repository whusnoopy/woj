#include "registerprocessimp.h"
#include "base/logging.h"
#include "base/flags.h"
#include "base/util.h"
#include "util/calulate.h"
#include "object/user.h"
#include "data/datainterface.h"

using namespace std;

void RegisterProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the data.";
  char* buf;
  User user;
  buf =  new char[length + 1];
  memset(buf, 0, length + 1);
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Not enough data.";
    delete[] buf;
    return ;
  }
  string data(buf);
  delete[] buf;
  vector<string> datalist;
  spriteString(data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id in data.";
    return;
  }
  user.setId(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find password in data.";
    return;
  }
  user.setPassword(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find email in data.";
    return;
  }
  user.setEmail(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find show_email in data.";
    return;
  }
  user.setShowEmail(*iter == "Y");
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find nickname in data.";
    return;
  }
  user.setNickname(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find school in data.";
    return;
  }
  user.setSchool(*iter);
  iter++;
  user.setSubmit(0);
  user.setSolved(0);
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find share_code in data.";
    return;
  }
  user.setShareCode(*iter == "Y");
  iter++;
  user.setVolume(0);
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find language in data.";
    return;
  }
  user.setLanguage(atoi((*iter).c_str()));
  iter++;
  user.setAvailable(true);
  user.setLastLoginIp(ip);
  user.setLastLoginTime(getLocalTimeAsString("%Y-%m-%d %H:%M:%S"));
  user.setRegTime(getLocalTimeAsString("%Y-%m-%d %H:%M:%S"));
  user.setPermission(0);
  user.setIndentifyCode(calIndentifyCode(user.getId()));
  DataInterface interface = DataInterface::getInstance();
  interface.addUser(user);
  if (sendReply(socket_fd, 's') != 0){
    LOG(ERROR) << "Cannot ack.";
    return;
  }
  LOG(INFO) << "Process the register completed for:" << ip;
}

