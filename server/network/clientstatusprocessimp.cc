#include "clientstatusprocessimp.h"

#include <vector>
#include <string>

#include "util/calulate.h"
#include "object/list.h"
#include "object/contest.h"
#include "object/problem.h"
#include "base/logging.h"
#include "base/flags.h"
#include "base/util.h"
#include "data/datainterface.h"
using namespace std;

void ClientStatusProcessImp::process(int socket_fd, const string& ip, int length) {
  LOG(INFO) << "Process the Client for:" << ip;
  char* buf;
  buf =  new char[length + 1];
  memset(buf, 0, length + 1);
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Not enough data.";
    delete[] buf;
    return ;
  }
  string read_data(buf);
  delete[] buf;
  string user_id = read_data;

  StatusList status_list = DataInterface::getInstance().getClientStatusList(user_id);
     
  string databuf;
  databuf = stringPrintf("%d", status_list.size());
  StatusList::iterator iter = status_list.begin();
  while (iter != status_list.end()) {
    databuf += stringPrintf("\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%s", 
                            iter->getStatusId(),
                            iter->getProblemId(),
                            iter->getContestId(),
                            iter->getResult(),
                            iter->getMemory(),
                            iter->getTime(),
                            iter->getLanguage(),
                            iter->getCodeLength(),
                            iter->getSubmitTime().c_str());
    iter++;
  }

  string len = stringPrintf("%010d\n", databuf.length());
  if (socket_write(socket_fd, len.c_str(), 11)) {
    LOG(ERROR) << "Cannot write data length to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot write source to:" << ip;
    return;
  }
  LOG(INFO) << "Process Client completed for:" << ip;
}

