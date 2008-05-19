#include "listjobdoneprocessimp.h"

#include <string>
#include <set>
#include <vector>

#include "object/student.h"
#include "object/info.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/teachinterface.h"
#include "object/job.h"
#include "object/student.h"
#include "object/list.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void ListJobDoneProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process list Job done Information for:" << ip;
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
    LOG(ERROR) << "Cannot find grade from data for:" << ip;
    return;
  }
  int grade = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find job_id from data for:" << ip;
    return;
  }
  int job_id = atoi(iter->c_str());
  iter++;
  string databuf;
  Job job = TeachInterface::getInstance().getJob(job_id);
  StudentList student_list = TeachInterface::getInstance().getStudentList(grade);
  StudentList::iterator student_iter = student_list.begin();
  while (student_iter != student_list.end()) {
    student_iter->is_done = checkJobDone(student_iter->user_id, job);
    student_iter++;
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
  LOG(INFO) << "Process list Job done Info completed for" << ip;
}

void ListJobDoneProcessImp::checkJobDone(const string& user_id, const Job& job) {
  ProblemSet ac_set = DataInterface::getInstance().getUserACProblem(user_id, true);
  vector<int> problem_list = job.getProblemList();
  vector<int>::iterator must_do_problem = problem_list.begin();
  while (must_do_problem != problem_list.end()) {
    if (ac_set.count(*must_do_problem) == 0) 
      return false;      
    must_do_problem++;
  }

  vector<SetItem> set_list = job.getSetList();
  vector<SetItem>::iterator should_do_set = set_list.begin();
  while (should_do_set != set_list.end()) {
    vector<int>::iterator should_do_problem = should_do_set->problem_list.begin();
    int done_should_problem = 0;
    while (should_do_problem != should_do_set->problem_list.end()) {
      if (ac_set.count(*should_do_problem) > 0) {
        done_should_problem++;
      } else 
      should_do_problem++;
    }
    if (done_should_problem < should_do_set->number) {
      return false;
    }
    should_do_set++;
  }
  return true;
}



