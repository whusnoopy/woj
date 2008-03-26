#include "ranklistprocessimp.h"

#include <string>
#include <vector>

#include "../object/info.h"
#include "../object/list.h"
#include "../util/calulate.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void RankListProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process RankList for:" << ip;
  char* buf;
  buf = new char[length+1];
  memset(buf,0,sizeof(buf));
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string read_data(buf);
  delete[] buf;
  vector<string> datalist;
  spriteString(read_data, 1, datalist);
  RankListInfo rankinfo;
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find page_id from data for:" << ip;
    return;
  }
  rankinfo.page_id = atoi(iter->c_str());
  iter++;

  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find of1 from data for:" << ip;
    return ;
  }
  rankinfo.seq = atoi(iter->c_str());
  
  RankList ranklist;
  //ranklist = DataInterface::getInstance().getRankList();
  RankList::iterator rank_iter = ranklist.begin();
  string databuf;
  bool first = true;
  while (rank_iter != ranklist.end()) {
    if (first) {
      databuf += stringPrintf("%s\001%s\001%d\001%d",
                              rank_iter->user_id.c_str(),
                              rank_iter->nickname.c_str(),
                              rank_iter->solved,
                              rank_iter->submit);
      first = false;
    }else {
      databuf += stringPrintf("\001%s\001%s\001%d\001%d",
                              rank_iter->user_id.c_str(),
                              rank_iter->nickname.c_str(),
                              rank_iter->solved,
                              rank_iter->submit);
    }
    rank_iter++;
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
  LOG(INFO) << "Process RankList completed for" << ip;
}


