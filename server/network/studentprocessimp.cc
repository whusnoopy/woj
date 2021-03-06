#include "studentprocessimp.h"

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

void StudentProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process Student Information for:" << ip;
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
  Student student = TeachInterface::getInstance().getStudent(user_id);
  string databuf;
  databuf += stringPrintf("%s\001%s\001%s\001%s\001%d\001%d\001%s",
                          student.getUserId().c_str(),
                          student.getStudentId().c_str(),
                          student.getRealName().c_str(),
                          "  ",
                          student.getClass().getGrade(),
                          student.getClass().getClass(),
                          student.getAvailable() ? "Y" :"N");  
  string len = stringPrintf("%010d", databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process Student Info completed for" << ip;
}

