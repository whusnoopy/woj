#include "submitdocprocessimp.h"

#include <vector>
#include <string>

#include "object/objectinc.h"
#include "object/bufsize.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "judgecontrol/judgecontrol.h"
using namespace std;

void SubmitDocProcessImp::process(int socket_fd, const string& ip, int length) {
  LOG(INFO) << "Process the Submit Doc for:" << ip;
  char* buf;
  buf = new char[length+1];
  memset(buf,0,sizeof(buf));
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string data(buf, buf + length);
  delete[] buf;
  vector<string> datalist;
  spriteString(data, 1, datalist);
  LOG(DEBUG) << data;
  vector<string>::iterator iter = datalist.begin();
  Status status;
  string user_id, password, submit_ip;
  int problem_id = 0, language = 0;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id from data for:" << ip;
    return;
  }
  user_id = *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find password from data for:" << ip;
    return;
  }
  password = *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find problem_id from data for:" << ip;
    return;
  }
  LOG(DEBUG) << *iter;
  problem_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find language from data for:" << ip;
    return;
  }
  language = atoi(iter->c_str());
  iter++;  
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find ip from data for:" << ip;
    return;
  }
  submit_ip = *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find type from data for:" << ip;
    return;
  }
  string type = *iter;
  iter++;
  //check user;
  User user = DataInterface::getInstance().getUserInfo(user_id);
  if (user.getPassword() != password) {
    sendReply(socket_fd, 'N');
    return;
  }
  status.setUserId(user_id);
  status.setProblemId(problem_id);
  status.setContestId(0);
  status.setLanguage(language);
  status.setCodeLength(0);
  status.setSubmitIp(submit_ip);
  status.setSubmitTime(getLocalTimeAsString("%Y-%m-%d %H:%M:%S"));
  status.setResult(0);
  status.setMemory(-1);
  status.setTime(-1);
  status.setCodeId(0);
  status.setErrorId(0);
  status.setType(type.substr(0,1));
  int status_id = DataInterface::getInstance().addStatus(status);
  string databuf = stringPrintf("%010d", status_id);
  if (socket_write(socket_fd, databuf.c_str(), 10)) {
    LOG(ERROR) << "Cannot return status id to " << ip;
    return;
  }
  LOG(INFO) << "Submit Doc process completed." << ip;
}

