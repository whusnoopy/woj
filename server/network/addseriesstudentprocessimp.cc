#include "addseriesstudentprocessimp.h"
#include "base/logging.h"
#include "base/flags.h"
#include "base/util.h"
#include "util/calulate.h"
#include "object/user.h"
#include "object/student.h"
#include "data/teachinterface.h"

using namespace std;

void AddSeriesStudentProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the add series student for:" << ip;
  char* buf;
  User user;
  Student student;
  buf =  new char[length + 1];
  memset(buf, 0, length + 1);
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Not enough data.";
    delete[] buf;
    return ;
  }
  string data(buf, buf + length);
  delete[] buf;
  vector<string> datalist;
  spriteString(data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  string databuf;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find number from data for" << ip;
    return;
  }
  int number = atoi(iter->c_str());
  iter++;
  for(int i = 0; i < number; i++) {
    if (iter == datalist.end()) {
      LOG(ERROR) << "Cannot find user_id in data.";
      return;
    }
    user.setId(*iter);
    student.setUserId(*iter);
    iter++;
    if (iter == datalist.end()) {
      LOG(ERROR) << "Cannot find password in data.";
      return;
    }
    user.setPassword(*iter);
    iter++;
    if (iter == datalist.end()) {
      LOG(ERROR) << "Cannot find student id in data";
      return;
    }
    student.setStudentId(*iter);
    iter++;
    if (iter == datalist.end()) {
      LOG(ERROR) << "Cannot find realname in data";
      return;
    }
    student.setRealName(*iter);
    iter++;
    if (iter == datalist.end()) {
      LOG(ERROR) << "Cannot find grade in data";
      return;
    }
    student.setGrade(atoi(iter->c_str()));
    iter++;
    if (iter == datalist.end()) {
      LOG(ERROR) << "Cannot find class_no in data";
      return;
    }
    student.setGrade(atoi(iter->c_str()));
    iter++;
    user.setEmail("system");
    user.setShowEmail(true);
    user.setNickname("system");
    user.setSchool("system");
    user.setSubmit(0);
    user.setSolved(0);
    user.setShareCode(false);
    user.setVolume(0);
    user.setLanguage(0);
    user.setAvailable(true);
    user.setLastLoginIp("0.0.0.0");
    user.setLastLoginTime(getLocalTimeAsString("%Y-%m-%d %H:%M:%S"));
    user.setRegTime(getLocalTimeAsString("%Y-%m-%d %H:%M:%S"));
    user.setPermission(0);
    user.setIndentifyCode(calIndentifyCode(user.getId()));
    DataInterface interface = DataInterface::getInstance();
    int ret = interface.addUser(user);
    ret &= TeachInterface::getInstance().addStudent(student);
    char c = 'Y';
    if (ret != 0)
      c = 'N';
    if (databuf.empty()) 
      databuf += stringPrintf("%c", c);
    else      
      databuf += stringPrintf("\001%c", c);
  }
  LOG(DEBUG) << databuf;
  string len = stringPrintf("%010d", databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)) {
    LOG(ERROR) << "Cannot return datalength to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot send add series student data to:" << ip;
    return;
  }
  LOG(INFO) << "Process the add series student completed for:" << ip;
}

