#include "contestlistprocessimp.h"

#include <string>
#include <vector>

#include "../object/contest.h"
#include "../object/info.h"
#include "../object/list.h"
#include "../util/calulate.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void ContestListProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the ContestList for:" << ip;
  char* buf;
  buf = new char[length+1];
  memset(buf,0,sizeof(buf));
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string data(buf);
  delete[] buf;
  vector<string> datalist;
  spriteString(data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  ContestInfo contest_info;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find page_id from data for:" << ip;
    return;
  }
  contest_info.page_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find contest_type from data for:" << ip;
    return;
  }
  contest_info.type = (*iter)[0];
  contest_info.title = string("NULL");
  contest_info.description = string("NULL");
  ContestList contest_list;
  //contest_list = DatabaseInterface::getInstance().getContestList();
  string databuf;
  ContestList::iterator contest_iter = contest_list.begin();
  bool first = true;
  while (contest_iter != contest_list.end()) {
    if (first) {
      first = false;
      databuf += stringPrintf("%d\001%s\001%d\001%d\001%s\001%d",
                              contest_iter->contest_id,
                              contest_iter->title.c_str(),
                              contest_iter->start_time.c_str(),
                              contest_iter->end_time.c_str(),
                              contest_iter->type.c_str(),
                              contest_iter->public_id);
    }else {
      databuf += stringPrintf("\001%d\001%s\001%d\001%d\001%s\001%d",
                              contest_iter->contest_id,
                              contest_iter->title.c_str(),
                              contest_iter->start_time.c_str(),
                              contest_iter->end_time.c_str(),
                              contest_iter->type.c_str(),
                              contest_iter->public_id);
    }
    contest_iter++;
  }
  
  string len = stringPrintf("%010d",databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process ContestList completed for" << ip;
}

