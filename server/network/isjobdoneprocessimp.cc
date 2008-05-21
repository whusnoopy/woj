#include "isjobdoneprocessimp.h"

#include <string>
#include <set>
#include <vector>

#include "object/student.h"
#include "object/info.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/teachinterface.h"
#include "data/datainterface.h"
#include "object/job.h"
#include "object/list.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void IsJobDoneProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process is Job done Information for:" << ip;
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
    LOG(ERROR) << "Cannot find user_id from data for:" << ip;
    return;
  }
  string user_id = *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find job_id from data for:" << ip;
    return;
  }
  int job_id = atoi(iter->c_str());
  iter++;
  string databuf;
  Job job = TeachInterface::getInstance().getJob(job_id);
  ProblemSet ac_set = DataInterface::getInstance().getUserACProblem(user_id, true);
  databuf += stringPrintf("%d", job.getJobId());
  vector<int> problem_list = job.getProblemList();
  int must_do_number = problem_list.size();
  databuf += stringPrintf("\001%d", must_do_number);
  vector<int>::iterator must_do_problem = problem_list.begin();
  int done_must_problem = 0;
  while (must_do_problem != problem_list.end()) {
    databuf += stringPrintf("\001%d", *must_do_problem);
    if (ac_set.count(*must_do_problem) > 0) {
      databuf += "\001Y";
      done_must_problem++;
    } else 
      databuf += "\001N";
    must_do_problem++;
  }
  databuf += stringPrintf("\001%d", done_must_problem);
  vector<SetItem> set_list = job.getSetList();
  int should_do_number = set_list.size();
  databuf += stringPrintf("\001%d", should_do_number);
  vector<SetItem>::iterator should_do_set = set_list.begin();
  while (should_do_set != set_list.end()) {
    databuf += stringPrintf("\001%d\001%d", should_do_set->number, should_do_set->problem_list.size());
    vector<int>::iterator should_do_problem = should_do_set->problem_list.begin();
    int done_should_problem = 0;
    while (should_do_problem != should_do_set->problem_list.end()) {
      databuf += stringPrintf("\001%d", *should_do_problem);
      if (ac_set.count(*should_do_problem) > 0) {
        databuf += "\001Y";
        done_should_problem++;
      } else 
        databuf += "\001N";
      should_do_problem++;
    }
    databuf += stringPrintf("\001%d", done_should_problem);
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
  LOG(INFO) << "Process is Job done Info completed for" << ip;
}

