#include "listjobdoneprocessimp.h"

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
    LOG(ERROR) << "Cannot find job_id from data for:" << ip;
    return;
  }
  int job_id = atoi(iter->c_str());
  iter++;
  string databuf;
  Job job = TeachInterface::getInstance().getJob(job_id);
  StudentList student_list = TeachInterface::getInstance().getStudentList(job.getCourseId());
  StudentList::iterator student_iter = student_list.begin();
  while (student_iter != student_list.end()) {
    LOG(DEBUG) << "score";
    student_iter->score = checkJobDone(student_iter->user_id, job);
    student_iter++;
  }
  student_iter = student_list.begin();
  bool first = true;
  while (student_iter != student_list.end()) {
    if (!first) 
      databuf += "\001";
    else
      first = false;
    LOG(DEBUG) << "ok";
    databuf += stringPrintf("%s\001%s\001%s\001%s\001%d\001%d",
                            student_iter->user_id.c_str(),
                            student_iter->realname.c_str(),
                            student_iter->student_id.c_str(),
                            student_iter->available ? "Y" : "N",
                            student_iter->mclass.getGrade(),
                            student_iter->score);
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

int ListJobDoneProcessImp::checkJobDone(const string& user_id, const Job& job) {
  ProblemSet ac_set = DataInterface::getInstance().getUserACProblem(user_id, true);
  vector<int> problem_list = job.getProblemList();
  vector<int>::iterator must_do_problem = problem_list.begin();
  int has_done_must_problem = 0; 
  while (must_do_problem != problem_list.end()) {
    if (ac_set.count(*must_do_problem) > 0)
      has_done_must_problem++;
    must_do_problem++;
  }
  int score = 60;
  if (has_done_must_problem < problem_list.size()) {
      score = has_done_must_problem * 60 / problem_list.size();
    return score;
  }

  vector<SetItem> set_list = job.getSetList();
  if (set_list.size() == 0) {
    return score + 40;
  }
  int should_score = 40 / set_list.size();
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
    if (done_should_problem < should_do_set->number) 
      score += should_score * done_should_problem / should_do_set->number; 
    else 
      score += should_score;
    should_do_set++;
  }
  return score;
}



