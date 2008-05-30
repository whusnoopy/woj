#include "jobprocessimp.h"

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

void JobProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process Job Information for:" << ip;
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
    LOG(ERROR) << "Cannot find job_id from data for:" << ip;
    return;
  }
  int job_id = atoi(iter->c_str());
  iter++;
  string databuf;
  Job job = TeachInterface::getInstance().getJob(job_id);
  databuf += stringPrintf("%d\001%s\001%s\001%d\001%d\001%c", 
                          job.getJobId(), 
                          job.getDescription().c_str(),
                          job.getPublishTime().c_str(),
                          job.getCourseId(),
                          job.getYear(),
                          job.getTerm());
  vector<int> problem_list = job.getProblemList();
  int must_do_number = problem_list.size();
  databuf += stringPrintf("\001%d", must_do_number);
  vector<int>::iterator must_do_problem = problem_list.begin();
  while (must_do_problem != problem_list.end()) {
    databuf += stringPrintf("\001%d", *must_do_problem);
    must_do_problem++;
  }
  vector<SetItem> set_list = job.getSetList();
  int should_do_number = set_list.size();
  databuf += stringPrintf("\001%d", should_do_number);
  vector<SetItem>::iterator should_do_set = set_list.begin();
  while (should_do_set != set_list.end()) {
    databuf += stringPrintf("\001%d\001%d\001%d", should_do_set->set_id, should_do_set->number, should_do_set->problem_list.size());
    vector<int>::iterator should_do_problem = should_do_set->problem_list.begin();
    while (should_do_problem != should_do_set->problem_list.end()) {
      databuf += stringPrintf("\001%d", *should_do_problem);
      should_do_problem++;
    }
    should_do_set++;
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
  LOG(INFO) << "Process Job Info completed for" << ip;
}

