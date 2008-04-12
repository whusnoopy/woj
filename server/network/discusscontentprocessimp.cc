#include "discusscontentprocessimp.h"

#include "object/discuss.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void DiscussContentProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the Discuss Content for:" << ip;
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
    LOG(ERROR) << "Cannot find discuss_id from data for:" << ip;
    return;
  }
  int discuss_id = atoi(iter->c_str());
  Discuss discuss;
  discuss = DataInterface::getInstance().getDiscuss(discuss_id);
  string databuf;
  databuf += stringPrintf("%d\001%s\001%d\001%d\001%s\001%s\001%s",
                          discuss.getTopicId(),
                          discuss.getTitle().c_str(),
                          discuss.getProblemId(),
                          discuss.getContestId(),
                          discuss.getUserId().c_str(),
                          discuss.getDate().c_str(),
                          discuss.getContent().c_str());

  string len = stringPrintf("%010d",databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process DiscussContent completed for" << ip;
}

