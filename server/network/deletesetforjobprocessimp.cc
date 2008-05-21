#include "deletesetforjobprocessimp.h"

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

void DeleteSetForJobProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process delete set for job for:" << ip;
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
    LOG(ERROR) << "Cannot find job_id from data for:" << ip;
    return;
  }
  int job_id  = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find set_id from data for:" << ip;
    return;
  }
  int set_id  = atoi(iter->c_str());
  iter++;
  int ret = TeachInterface::getInstance().deleteSetForJob(job_id, set_id);
  if (ret) {
    sendReply(socket_fd, 'N');
    LOG(ERROR) << "delete set for job Error";
    return;
  }
  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot reply to " << ip;
    return;
  }
  LOG(INFO) << "Process delete set for job completed for:" << ip;
}

