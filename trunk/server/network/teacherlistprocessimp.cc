#include "teacherlistprocessimp.h"

#include <string>
#include <set>
#include <vector>

#include "object/student.h"
#include "object/info.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/teachinterface.h"
#include "object/teacher.h"
#include "object/list.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void TeacherListProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process Teacher list Information for:" << ip;
  string databuf;
  TeacherList teacher_list = TeachInterface::getInstance().getTeacherList();
  TeacherList::iterator teacher_iter = teacher_list.begin();
  bool first = true;
  while (teacher_iter != teacher_list.end()) {
    if (!first) 
      databuf += "\001";
    else
      first = false;
    databuf += stringPrintf("%s\001%s\001%s",
                            teacher_iter->getUserId().c_str(),
                            teacher_iter->getRealName().c_str(),
                            teacher_iter->getAvailable() ? "Y" : "N");
    teacher_iter++;
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
  LOG(INFO) << "Process teacher list Info completed for" << ip;
}

