#include "addseriesstudenttocourseprocessimp.h"

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

void AddSeriesStudentToCourseProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process add Series Student to course for:" << ip;
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
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find course_id from data for:" << ip;
    return;
  }
  int course_id = atoi(iter->c_str());
  iter++;
  bool first = true;
  string databuf;
  while (iter != datalist.end()) {
    if (first)
      first = false;
    else
      databuf += "\001";
    string user_id = *iter;
    int ret = TeachInterface::getInstance().addStudentToCourse(user_id, course_id);
    if (ret)
      databuf += "N";
    else 
      databuf += "Y";
    iter++;
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
  LOG(INFO) << "Process add Series Student to course for:" << ip;
}

