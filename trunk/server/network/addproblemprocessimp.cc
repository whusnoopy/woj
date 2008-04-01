#include "addproblemprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "data/datainterface.h"
#include "util/calulate.h"
#include "object/problem.h"
#include "object/list.h"
#include "object/info.h"
#include "object/user.h"
using namespace std;

void AddProblemProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process add problem for:" << ip;
  char* buf;
  buf = new char[length + 1];
  memset(buf, 0, length + 1);
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string read_data(buf);
  delete[] buf;
  vector<string> datalist;
  spriteString(read_data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  Problem problem;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot title find from data for:" << ip;
    return;
  }
  problem.setTitle(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find description from data for:" << ip;
    return;
  }
  problem.setDescription(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find input from data for:" << ip;
    return;
  }
  problem.setInput(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find output from data for:" << ip;
    return;
  }
  problem.setOutput(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find sample_input from data for:" << ip;
    return;
  }
  problem.setSampleInput(*iter);
  iter++; 
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find sample_output from data for:" << ip;
    return;
  }
  problem.setSampleOutput(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find hint from data for:" << ip;
    return;
  }
  problem.setHint(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find source from data for:" << ip;
    return;
  }
  problem.setSource(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find time_limit from data for:" << ip;
    return;
  }
  problem.setTimeLimit(atoi(iter->c_str()));
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find case_time_limit from data for:" << ip;
    return;
  }
  problem.setCaseTimeLimit(atoi(iter->c_str()));
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find memory_limit from data for:" << ip;
    return;
  }
  problem.setMemoryLimit(atoi(iter->c_str()));
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find spj from data for:" << ip;
    return;
  }
  problem.setSpj(*iter == "Y");
  iter++;
  problem.setAccepted(0);
  problem.setSubmit(0);
  problem.setSolvedUsers(0);
  problem.setSubmitUsers(0);
  problem.setStandardTimeLimit(0);
  problem.setStandardMemoryLimit(0);
  problem.setAvailable(true);
  problem.setVersion(1);
  int problem_id = 0;
  problem_id = DataInterface::getInstance().addProblem(problem);
  string databuf = stringPrintf("%010d", problem_id);
  
  if (socket_write(socket_fd, databuf.c_str(), 10)) {
    LOG(ERROR) << "Cannot return problem_id to:" << ip;
    return;
  }

  LOG(ERROR) << "Process add problem completed for:" << ip;
}

