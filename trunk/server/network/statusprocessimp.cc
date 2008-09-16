#include "statusprocessimp.h"

#include <string>
#include <vector>

#include "object/status.h"
#include "object/code.h"
#include "object/user.h"
#include "object/list.h"
#include "object/info.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void StatusProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(DEBUG) << "Process the Status for:" << ip;
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
  StatusInfo status_info;
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find page_id for:" << ip;
    return ;
  }
  status_info.page_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find problem_id for:" << ip;
    return ;
  }
  status_info.problem_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id for:" << ip;
    return ;
  }
  status_info.user_id = (*iter == "?")?"NULL":*iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find result for:" << ip;
    return ;
  }
  status_info.result = (*iter == "?")?-1:atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find language for:" << ip;
    return ;
  }
  status_info.language = (*iter == "?")?-1:atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find contest_id for:" << ip;
    return ;
  }
  status_info.contest_id = (*iter == "?")?0:atoi(iter->c_str());
  iter++;

  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find share_code for:" << ip;
    return ;
  }
  status_info.share_code_enable = (*iter == "Y");
  if (status_info.share_code_enable) {
    status_info.share_code = true;
  }
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find type for:" << ip;
    return;
  }
  string type = *iter;
  iter++;
  LOG(DEBUG) << "type:" << type;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id for:" << ip;
    return;
  }
  string user_id = *iter;
  iter++;
  bool hasuser = false;
  if (user_id != "?")
    hasuser = true;
  User user;
  ProblemSet acset;
  if (hasuser) {
    user = DataInterface::getInstance().getUserInfo(user_id);
    acset = DataInterface::getInstance().getUserACProblem(user_id, true);
  }
  LOG(DEBUG) << "user_id" << user_id;
  StatusList status_list;
  status_info.type = "NULL";
  if (type != "P"){
    status_info.type = type;
    status_list = DataInterface::getInstance().getSearchStatus(status_info);
    LOG(DEBUG) << "process st for:" << ip;
  }else {
    status_list = DataInterface::getInstance().getProblemStatus(status_info);
    LOG(DEBUG) << "process ps for:" << ip;
  }
  
  StatusList::iterator status_iter = status_list.begin();
  string databuf;
  bool first = true;
  while (status_iter != status_list.end()) {
    if (first) {
      databuf += stringPrintf("%d\001%s\001%d\001%d\001%d\001%d\001%d\001%d\001%s\001%d\001%d", 
                             status_iter->getStatusId(),
                             status_iter->getUserId().c_str(),
                             status_iter->getProblemId(),
                             status_iter->getResult(),
                             status_iter->getMemory(),
                             status_iter->getTime(),
                             status_iter->getLanguage(),
                             status_iter->getCodeLength(),
                             status_iter->getSubmitTime().c_str(),
                             status_iter->getCodeId(),
                             status_iter->getErrorId());
      if (hasuser) {
        Code code;
        code = DataInterface::getInstance().getCode(status_iter->getCodeId());
        if (user.getPermission() & 0x01 || user.getId() == status_iter->getUserId() 
            || (code.getShare() && acset.count(status_iter->getProblemId()))) {
          databuf += "\001Y";
        }else 
          databuf += "\001N";
      }else
        databuf += "\001N";
      first = false;
    } else {
      databuf += stringPrintf("\001%d\001%s\001%d\001%d\001%d\001%d\001%d\001%d\001%s\001%d\001%d", 
                             status_iter->getStatusId(),
                             status_iter->getUserId().c_str(),
                             status_iter->getProblemId(),
                             status_iter->getResult(),
                             status_iter->getMemory(),
                             status_iter->getTime(),
                             status_iter->getLanguage(),
                             status_iter->getCodeLength(),
                             status_iter->getSubmitTime().c_str(),
                             status_iter->getCodeId(),
                             status_iter->getErrorId());
      if (hasuser) {
        Code code;
        code = DataInterface::getInstance().getCode(status_iter->getCodeId());  
        if (user.getPermission() & 0x01 || user.getId() == status_iter->getUserId() 
            || (code.getShare() && acset.count(status_iter->getProblemId()))){
          databuf += "\001Y";
        }else 
          databuf += "\001N";
      }else
        databuf += "\001N";
    }
    status_iter++;
  }
  LOG(DEBUG) << databuf; 
  string len = stringPrintf("%010d",databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(DEBUG) << "Process Status completed for" << ip;
}

