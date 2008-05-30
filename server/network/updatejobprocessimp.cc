#include "updatejobprocessimp.h"

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

void UpdateJobProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process update Job for:" << ip;
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
    LOG(ERROR) << "Cannot find job_id from data for:" << ip;
    return;
  }
  job.setJobId(atoi(iter->c_str()));
  iter++;
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
  job.setTerm(*iter->c_str());
  iter++;
  int ret = TeachInterface::getInstance().updateJob(job);
  if (ret) {
    sendReply(socket_fd, 'N');
    LOG(ERROR) << "Update Job Error";
    return;
  }
  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot reply to " << ip;
    return;
  }
  LOG(INFO) << "Process update Job completed for:" << ip;
}

