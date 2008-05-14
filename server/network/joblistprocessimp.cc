#include "joblistprocessimp.h"

#include <string>
#include <set>
#include <vector>

#include "object/student.h"
#include "object/info.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/teachinterface.h"
#include "object/job.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void JobListProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process Job List Information for:" << ip;
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
    LOG(ERROR) << "Cannot find teacher from data for:" << ip;
    return;
  }
  string teacher = *iter;
  iter++;
  string databuf;
  JobList job_list = TeachInterface::getInstance().getJobList(teacher);
  bool first = true;
  JobList::iterator job_iter = job_list.begin();
  while (job_iter != job_list.end()) {
    if (!first) 
      databuf += "\001";
    else 
      first = false;
    databuf += stringPrintf("%d\001%s\001%s\001%s\001%d\001%c", 
                            job_iter->job_id, 
                            job_iter->description.c_str(),
                            job_iter->publish_time.c_str(),
                            job_iter->teacher.c_str(),
                            job_iter->year,
                            job_iter->term);
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
  LOG(INFO) << "Process Job List Info completed for" << ip;
}

