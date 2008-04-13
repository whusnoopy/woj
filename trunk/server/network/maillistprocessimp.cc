#include "maillistprocessimp.h"

#include <string>
#include <vector>

#include "object/mail.h"
#include "object/info.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void MailListProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process Mail List for:" << ip;
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
  MailInfo mail_info;
  mail_info.title = string("NULL");
  spriteString(read_data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id from:" << ip;
    return;
  }
  mail_info.user_id = *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find page_id from" << ip;
    return;
  }
  mail_info.page_id = atoi(iter->c_str());
  MailList mail_list;
  mail_list = DataInterface::getInstance().getMailList(mail_info);
  string databuf;
  MailList::iterator mail_iter = mail_list.begin();
  bool first = true;
  while (mail_iter != mail_list.end()) {
    if (first) {
      databuf += stringPrintf("%d\001%s\001%s\001%s\001%s\001%s", 
                              mail_iter->mail_id, 
                              mail_iter->to_user.c_str(),
                              mail_iter->from_user.c_str(), 
                              mail_iter->title.c_str(),
                              mail_iter->date.c_str(), 
                             (mail_iter->read)?"Y":"N");
      first = false;
    } else {
      databuf += stringPrintf("\001%d\001%s\001%s\001%s\001%s\001%s",
                              mail_iter->mail_id, 
                              mail_iter->to_user.c_str(),
                              mail_iter->from_user.c_str(), 
                              mail_iter->title.c_str(),
                              mail_iter->date.c_str(), 
                              (mail_iter->read)?"Y":"N"); 
    }
    mail_iter++;
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
  LOG(INFO) << "Process MailList completed for" << ip;
}

