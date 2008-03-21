#include "addmailprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "../util/calulate.h"
#include "../object/mail.h"
#include "../object/list.h"
#include "../object/info.h"
#include "../object/user.h"
using namespace std;

void AddMailProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process add Mail for:" << ip;
  char* buf;
  buf = new char[length + 1];
  memset(buf, 0, length + 1);
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
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find the top_id from:" << ip;
    return;
  }
  Mail mail;
  mail.setTopicId(atoi(iter->c_str()));
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Canot find title from:" << ip;
    return;
  }
  mail.setTitle(*iter);
  iter++;
  if (iter == datalist.end()){
    LOG(ERROR) << "Cannot find content from:" << ip;
    return;
  }
  mail.setContent(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find to_user from:" << ip;
    return;
  }
  mail.setToUser(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find from_user from:" << ip;
  }
  mail.setFromUser(*iter);
  mail.setRead(false);
  mail.setTime(getLocalTimeAsString("%Y-%m-%d %H:%M:%S"));
  mail.setReaderDel(false);
  mail.setWriterDel(false);
  User user;
  //user = DatabaseInterface::getInstance().getUserInfo(mail.getToUser());
  if (user.getId() == string("NULL")) {
    sendReply(socket_fd, 'N');
    return;
  }
  int ret = 0;
  //ret = DatabaseInterface::getInstance().addMail(mail);
  if (ret) {
    sendReply(socket_fd, 'N');
    return;
  }
  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot reply to:" << ip;
    return;
  }
  LOG(ERROR) << "Process addmail completed for:" << ip;
}

