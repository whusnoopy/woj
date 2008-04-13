#include "admincontestlistprocessimp.h"

#include <string>
#include <vector>

#include "object/contest.h"
#include "object/info.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void AdminContestListProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the Admin ContestList for:" << ip;
  char* buf;
  buf = new char[length+1];
  memset(buf,0,sizeof(buf));
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string data(buf, buf + length);
  delete[] buf;
  vector<string> datalist;
  spriteString(data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find page_id from data for:" << ip;
    return;
  }
  ContestInfo contest_info;
  contest_info.title = "NULL";
  contest_info.description = "NULL";
  contest_info.type = 'C';
  contest_info.page_id = atoi(iter->c_str());
  ContestList contest_list;
  contest_list = DataInterface::getInstance().getContestList(contest_info);
  string databuf;
  ContestList::iterator contest_iter = contest_list.begin();
  bool first = true;
  while (contest_iter != contest_list.end()) {
    if (first) {
      first = false;
      databuf += stringPrintf("%d\001%s\001%s\001%s\001%s\001%d\001%s",
                              contest_iter->contest_id,
                              contest_iter->title.c_str(),
                              contest_iter->start_time.c_str(),
                              contest_iter->end_time.c_str(),
                              contest_iter->type.c_str(),
                              contest_iter->public_id,
                              contest_iter->available?"Y":"N");
    }else {
      databuf += stringPrintf("\001%d\001%s\001%s\001%s\001%s\001%d\001%s",
                              contest_iter->contest_id,
                              contest_iter->title.c_str(),
                              contest_iter->start_time.c_str(),
                              contest_iter->end_time.c_str(),
                              contest_iter->type.c_str(),
                              contest_iter->public_id,
                              contest_iter->available?"Y":"N");
    }
    contest_iter++;
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
  LOG(INFO) << "Process Admin ContestList completed for" << ip;
}

