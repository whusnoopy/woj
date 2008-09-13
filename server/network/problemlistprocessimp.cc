#include "problemlistprocessimp.h"

#include <string>
#include <vector>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "util/calulate.h"
#include "object/problem.h"
#include "object/user.h"
#include "object/list.h"
#include "object/info.h"
#include "data/datainterface.h"
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
  string data(buf, buf + length);
  delete[] buf;
  vector<string> datalist;
  LOG(DEBUG) << data;
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
  //if (iter == datalist.end()) {
  //  LOG(ERROR) << "Cannot find indentify code from:" << ip;
  //  return;
  //}
  //string indentify_code = *iter;
  //iter++;

  int num = DataInterface::getInstance().getProblemListNum(problem_info);
  LOG(DEBUG) << "Generate page_id = " << problem_info.page_id
             << ", title = " << problem_info.title
             << ", source = " << problem_info.source
             << ", user_id = " << user_id;
  
  ProblemList list;
  ProblemList::iterator list_iter;
  if (user_id != "?") {
    User user;
    user = DataInterface::getInstance().getUserInfo(user_id);
    
    // if a login user view problem list without appoint a certain page,
    // give him the last page he stayed, or update his last stay page
    if (problem_info.page_id == -1) {
      problem_info.page_id = user.getVolume();
    } else {
      user.setVolume(problem_info.page_id);
      if (DatabaseInterface::getInstance().updateUser(user)) {
        LOG(ERROR) << "Cannot set user's last stayed page";
      }
    }
     list = DataInterface::getInstance().getProblemList(problem_info);
     LOG(DEBUG) << "Get problem list ok";

     ProblemSet ac_set; 
     ac_set = DataInterface::getInstance().getUserACProblem(user_id, true);
     LOG(DEBUG) << "Get user's ac set ok";

     ProblemSet nac_set;
     nac_set = DataInterface::getInstance().getUserACProblem(user_id, false);
     LOG(DEBUG) << "Get user's non-ac set ok";
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
  } else {
    if (problem_info.page_id == -1)
      problem_info.page_id =0 ;
    list = DataInterface::getInstance().getProblemList(problem_info);
    LOG(DEBUG) << "Here is ok";
  }
  LOG(DEBUG) << list.size();

  string databuf;
  list_iter = list.begin();
  bool first = true;
  while (list_iter != list.end()) {
    if (!list_iter->available) {
      list_iter++;
      continue;
    }
    if (first) {
      databuf += stringPrintf("%d\001%d\001%s\001%s\001%d\001%d\001%d", 
                              num,
                              list_iter->problem_id,
                              list_iter->title.c_str(),
                              list_iter->source.c_str(),
                              list_iter->accepted,
                              list_iter->submit,
                              list_iter->ac);
      first = false;
    } else {
      databuf += "\001" + 
                 stringPrintf("%d\001%s\001%s\001%d\001%d\001%d",
                              list_iter->problem_id,
                              list_iter->title.c_str(),
                              list_iter->source.c_str(),
                              list_iter->accepted,
                              list_iter->submit,
                              list_iter->ac);
    }
    list_iter++;
  }
  LOG(DEBUG) << databuf; 
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

