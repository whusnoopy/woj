#include "mailcontentprocessimp.h"

#include "../object/mail.h"
#include "../util/calulate.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void MailContentProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the Mail Content for:" << ip;
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
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find mail_id from data for:" << ip;
    return;
  }
  //int mail_id = atoi(iter->c_str());
  Mail mail;
  //mail = DatabaseInterface::getInstance().getMail(mail_id);
  //if (!mail.getRead()){
  //  mail.setRead(true);
  //  DatabaseInterface::getInstance().setMailRead(mail);
  //}
  string databuf;
  databuf = stringPrintf("%s\001%s\001%s\001%s\001%s", mail.getToUser().c_str(), 
                                                       mail.getFromUser().c_str(), 
                                                       mail.getTitle().c_str(), 
                                                       mail.getTime().c_str(),
                                                       mail.getContent().c_str());
  string len = stringPrintf("%010d",databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process MailContent completed for" << ip;
}

