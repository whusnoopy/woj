#include "courseprocessimp.h"

#include <string>
#include <set>
#include <vector>

#include "object/course.h"
#include "object/info.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/teachinterface.h"
#include "data/datainterface.h"
#include "object/job.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void CourseProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process course Information for:" << ip;
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
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find course_id from data for:" << ip;
    return;
  }
  int course_id = atoi(iter->c_str());
  iter++;
  Course course = TeachInterface::getInstance().getCourse(course_id);
  string databuf;
  databuf += stringPrintf("%d\001%s\001%s", 
                          course.getCourseId(), 
                          course.getDescription().c_str(),
                          course.getTeacher().c_str());
  string len = stringPrintf("%010d", databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process course completed for" << ip;
}

