#include "adminproblemlistprocessimp.h"

#include <string>
#include <vector>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "data/datainterface.h"
#include "util/calulate.h"
#include "object/problem.h"
#include "object/user.h"
#include "object/list.h"
#include "object/info.h"
using namespace std;

void AdminProblemListProcessImp::process(int socket_fd, const string& ip, int length) {
  LOG(INFO) << "Process the admin problemlist from:" << ip;
  char * buf;
  buf = new char[length + 1];
  memset(buf, 0, length + 1);
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string data(buf);
  delete[] buf;
  vector<string> datalist;
  spriteString(data, 1, datalist);
  ProblemInfo problem_info;
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find page_id from:" << ip;
    return;
  }
  problem_info.page_id = atoi(iter->c_str());
  iter++;  
  problem_info.problem_id = 0; 
  problem_info.title = "NULL";
  problem_info.source = "NULL";
  problem_info.related_contest = 0;
  ProblemList list;
  ProblemList::iterator list_iter;
  list = DatabaseInterface::getInstance().getProblemList(problem_info);

  string databuf;
  list_iter = list.begin();
  bool first = true;
  while (list_iter != list.end()) {
    if (first) {
      databuf += stringPrintf("%d\001%s\001%d\001%d\001%s", 
                              list_iter->problem_id,
                              list_iter->title.c_str(),
                              list_iter->accepted,
                              list_iter->submit,
                              list_iter->available?"Y":"N");
      first = false;
    } else {
      databuf += "\001" + 
                 stringPrintf("%d\001%s\001%d\001%d\001%s",
                              list_iter->problem_id,
                              list_iter->title.c_str(),
                              list_iter->accepted,
                              list_iter->submit,
                              list_iter->available?"Y":"N");
    }
    list_iter++;
  }
  
  string len = stringPrintf("%010d", databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)) {
    LOG(ERROR) << "Cannot return datalength to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot send problemlist data to:" << ip;
    return;
  }
  LOG(INFO) << "Process admin problemlist completed for:" << ip;
}

