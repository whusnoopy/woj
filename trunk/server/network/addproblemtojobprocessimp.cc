#include "addproblemtojobprocessimp.h"

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

void AddProblemToJobProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process add Problem to Job for:" << ip;
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
    LOG(ERROR) << "Cannot find type from data for:" << ip;
    return;
  }
  string type = *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find job_id from data for:" << ip;
    return;
  }
  int job_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find should do number from data for:" << ip;
    return;
  }
  int should_do_number = atoi(iter->c_str());
  LOG(DEBUG) << "should_do_number:" <<should_do_number;
  iter++;
  vector<int> problem_list;
  while (iter != datalist.end()) {
    problem_list.push_back(atoi(iter->c_str()));
    iter++;
  }
  int ret;
  if (type == "M") {
    ret = TeachInterface::getInstance().addProblemToJob(job_id, problem_list);
  } else {
    int set_id = TeachInterface::getInstance().addSet(problem_list, should_do_number);
    ret = TeachInterface::getInstance().addSetToJob(job_id, set_id);
  }
  if (ret) {
    sendReply(socket_fd, 'N');
    LOG(ERROR) << "add problem to job error";
    return;
  }
  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot reply to " << ip;
    return;
  }
  LOG(INFO) << "Process add Problem to Job completed for:" << ip;
}

