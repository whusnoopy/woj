#include "checkstudentorteacherprocessimp.h"

#include <string>
#include <set>
#include <vector>

#include "object/student.h"
#include "object/info.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/teachinterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void CheckStudentOrTeacherProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process Check Student or Teacher for:" << ip;
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
    LOG(ERROR) << "Cannot find type from data for:" << ip;
    return;
  }
  string type = *iter;
  iter++;
  bool ret = false;
  if (type == "S")
    ret = TeachInterface::getInstance().isStudent(user_id);
  else 
    ret = TeachInterface::getInstance().isTeacher(user_id);

  if (!ret) {
    sendReply(socket_fd, 'N');
    LOG(ERROR) << "Is not a student or teacher" << ip; 
    return;
  }
  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot reply to " << ip;
    return;
  }
  LOG(INFO) << "Process Check studnet or teacher completed for" << ip;
}

