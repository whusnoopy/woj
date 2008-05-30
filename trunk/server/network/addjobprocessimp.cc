#include "addjobprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "data/teachinterface.h"
#include "util/calulate.h"
#include "object/job.h"
#include "object/list.h"
#include "object/info.h"
using namespace std;

void AddJobProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process add Job for:" << ip;
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
  Job job;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find description from data for:" << ip;
    return;
  }
  job.setDescription(*iter);
  iter++;
  job.setPublishTime(getLocalTimeAsString("%Y-%m-%d %H:%M:%S"));
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find course_id from data for:" << ip;
    return;
  }
  job.setCourseId(atoi(iter->c_str()));
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find year from data for:" << ip;
    return;
  }
  job.setYear(atoi(iter->c_str()));
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find term from data for:" << ip;
    return;
  }
  job.setTerm(*(iter->c_str()));
  iter++;   
  job.setAvailable(true);
  int job_id = TeachInterface::getInstance().addJob(job);
  string databuf = stringPrintf("%010d", job_id);
  if (socket_write(socket_fd, databuf.c_str(), 10)) {
    LOG(ERROR) << "Cannot return job id to " << ip;
    return;
  }
  LOG(INFO) << "Process add Job completed for:" << ip;
}

