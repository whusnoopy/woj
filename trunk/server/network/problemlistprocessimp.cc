#include "problemlistprocessimp.h"

#include <string>
#include <vector>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "../util/calulate.h"
#include "../object/problem.h"
#include "../object/user.h"
#include "../object/list.h"
#include "../object/info.h"
using namespace std;

void ProblemListProcessImp::process(int socket_fd, const string& ip, int length) {
  LOG(INFO) << "Process the problemlist from:" << ip;
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
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find problem_id from:" << ip;
    return;
  }
  problem_info.problem_id = atoi(iter->c_str());
  iter++;
  // not done 
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find title from:" << ip;
    return;
  }
  problem_info.title = (*iter == "?")?string("NULL"):*iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find source from:" << ip;
    return;
  }
  problem_info.source = (*iter == "?")?string("NULL"):*iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find related contest from:" << ip;
    return;
  }
  problem_info.related_contest = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id from:" << ip;
    return;
  }
  string user_id = *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find indentify code from:" << ip;
    return;
  }
  string indentify_code = *iter;
  iter++;
  ProblemList list;
  ProblemList::iterator list_iter;
  if (user_id != "?") {
     User user;
     //user = DatabaseInterface::getInstance().getUserInfo(user_id);
     if (indentify_code == user.getIndentifyCode()) {
       problem_info.page_id = user.getVolume();
       //list = DatabaseInterface::getInstance().getProblemList(problem_info);
       ProblemSet ac_set; 
       //ac_set = DatabaseInterface::getInstance().getUseACProblem(user_id, true);
       ProblemSet nac_set;
       //nac_set = DatabaseInterface::getInstance().getUserACProblem(user_id, false);
       list_iter = list.begin();
       while (list_iter != list.end()){
         if (ac_set.count(list_iter->problem_id) == 1) {
           list_iter->ac = 1;
         }
         else if(nac_set.count(list_iter->problem_id) == 1) {
           list_iter->ac =2;
         }
         list_iter++;
       }
     }else {
       //list = DatabaseInterface::getInstance().getProblemList(problem_info);
     }
  }else {
    //list = DatabaseInterface::getInstance().getProblemList(problem_info);
  }


  string databuf;
  list_iter = list.begin();
  bool first = true;
  while (list_iter != list.end()) {
    if (!list_iter->available) {
      list_iter++;
      continue;
    }
    if (first) {
      databuf += stringPrintf("%d\001%s\001%d\001%d\001%d", 
                              list_iter->problem_id,
                              list_iter->title.c_str(),
                              list_iter->accepted,
                              list_iter->submit,
                              list_iter->ac);
      first = false;
    } else {
      databuf += "\001" + 
                 stringPrintf("%d\001%s\001%d\001%d\001%d",
                              list_iter->problem_id,
                              list_iter->title.c_str(),
                              list_iter->accepted,
                              list_iter->submit,
                              list_iter->ac);
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
  LOG(INFO) << "Process problemlist completed for:" << ip;
}

