#include "addstudentprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "data/teachinterface.h"
#include "util/calulate.h"
#include "object/student.h"
#include "object/list.h"
#include "object/info.h"
using namespace std;

void AddStudentProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process add Student for:" << ip;
  char* buf;
  buf = new char[length + 1];
  memset(buf, 0, length + 1);
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
  Student student;
  Class mclass;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id from data for:" << ip;
    return;
  }
  student.setUserId(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find student_id from data for:" << ip;
    return;
  }
  student.setStudentId(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find realname from data for:" << ip;
    return;
  }
  student.setRealName(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find college from data for:" << ip;
    return;
  }
  mclass.setCollege(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find grade from data for:" << ip;
    return;
  }
  mclass.setGrade(atoi(iter->c_str()));
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find class_no from data for:" << ip;
    return;
  }
  mclass.setClass(atoi(iter->c_str()));
  student.setClass(mclass);
  iter++;
  student.setAvailable(true);
  int ret = TeachInterface::getInstance().addStudent(student);
  if (ret) {
    sendReply(socket_fd, 'N');
    LOG(ERROR) << "add Student error";
    return;
  }
  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot reply to " << ip;
    return;
  }
  LOG(INFO) << "Process add problem completed for:" << ip;
}

