#include "contestcontentprocessimp.h"

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

void ContestContentProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the ContestContent for:" << ip;
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
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find contest_id from data for:" << ip;
    return;
  }
  //int contest_id = atoi(iter->c_str());
  Contest contest;
  //contest = DatabaseInterface::getInstance().getContestList();
  string databuf;
  databuf += stringPrintf("%s\001%s\001%s\001%s\001%d\001%d",
                          contest.getTitle().c_str(),
                          contest.getDescription().c_str(),
                          contest.getStartTime().c_str(),
                          contest.getEndTime().c_str(),
                          contest.getPublicId(),
                          contest.getVersion());
   
  string len = stringPrintf("%010d",databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process ContestContent completed for" << ip;
}

