#include "studentlistprocessimp.h"

#include <string>
#include <set>
#include <vector>

#include "object/student.h"
#include "object/info.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/teachinterface.h"
#include "object/student.h"
#include "object/list.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void StudentListProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process Student list Information for:" << ip;
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
    LOG(ERROR) << "Cannot find grade from data for:" << ip;
    return;
  }
  int grade = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find class_no from data for:" << ip;
    return;
  }
  int class_no = atoi(iter->c_str());
  iter++;
  string databuf;
  StudentList student_list = TeachInterface::getInstance().getStudentList(grade, class_no);
  StudentList::iterator student_iter = student_list.begin();
  bool first = true;
  while (student_iter != student_list.end()) {
    if (!first) 
      databuf += "\001";
    else
      first = false;
    databuf += stringPrintf("%s\001%s\001%s\001%s\001%d\001%d",
                            student_iter->user_id.c_str(),
                            student_iter->realname.c_str(),
                            student_iter->student_id.c_str(),
                            student_iter->available ? "Y" : "N",
                            student_iter->mclass.getGrade(),
                            student_iter->mclass.getClass());
    student_iter++;
  }
  string len = stringPrintf("%010d", databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process Student list Info completed for" << ip;
}

