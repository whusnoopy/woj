#include "statuslistforteacherprocessimp.h"

#include <string>
#include <vector>

#include "object/status.h"
#include "object/code.h"
#include "object/user.h"
#include "object/list.h"
#include "object/info.h"
#include "util/calulate.h"
#include "data/teachinterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void StatusListForTeacherProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the Status for:" << ip;
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
  if (iter ==  datalist.end()) {
    LOG(DEBUG) << "Cannot find job_id from data for:" << ip;
    return;
  }
  int job_id = atoi(iter->c_str());
  iter++;
  if (iter ==  datalist.end()) {
    LOG(DEBUG) << "Cannot find page_id from data for:" << ip;
    return;
  }
  int page_id = atoi(iter->c_str());
  iter++;
  StatusList status_list = TeachInterface::getInstance().getStatusListForTeacher(job_id, page_id);
  StatusList::iterator status_iter = status_list.begin();
  string databuf;
  bool first = true;
  while (status_iter != status_list.end()) {
    if (first) {
      first = false;
    } else 
      databuf += "\001";
    databuf += stringPrintf("%d\001%s\001%d\001%d\001%d\001%d\001%d\001%d\001%s\001%d\001%d", 
                            status_iter->getStatusId(),
                            status_iter->getUserId().c_str(),
                            status_iter->getProblemId(),
                            status_iter->getResult(),
                            status_iter->getMemory(),
                            status_iter->getTime(),
                            status_iter->getLanguage(),
                            status_iter->getCodeLength(),
                            status_iter->getSubmitTime().c_str(),
                            status_iter->getCodeId(),
                            status_iter->getErrorId());
    status_iter++;
  }
  LOG(DEBUG) << databuf; 
  string len = stringPrintf("%010d",databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process Status list for teacher completed for" << ip;
}

