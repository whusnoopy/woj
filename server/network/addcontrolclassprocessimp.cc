#include "addcontrolclassprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "data/teachinterface.h"
#include "util/calulate.h"
#include "object/teacher.h"
#include "object/class.h"
#include "object/list.h"
#include "object/info.h"
using namespace std;

void AddControlClassProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process add Control course for:" << ip;
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
  Class mclass;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find teacher_id from data for:" << ip;
    return;
  }
  string user_id = *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find course description from data for:" << ip;
    return;
  }
  string description = *iter;
  iter++;
  int course_id = TeachInterface::getInstance().addControlClass(user_id, description);
  string databuf = stringPrintf("%010d", course_id);
  if (socket_write(socket_fd, databuf.c_str(), 10)) {
    LOG(ERROR) << "Cannot return course id to " << ip;
    return;
  }
  LOG(INFO) << "Process add Control course completed for:" << ip;
}

