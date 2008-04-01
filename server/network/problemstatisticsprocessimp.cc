#include "problemstatisticsprocessimp.h"

#include <string>
#include <vector>

#include "object/status.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void ProblemStatisticsProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the ProblemStatistics for:" << ip;
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
    LOG(ERROR) << "Cannot find problem_id from data for:" << ip;
    return;
  }
  int problem_id = atoi(iter->c_str());
  ProblemStatistics problem_statistics;
  memset(&problem_statistics, 0, sizeof(problem_statistics));
  problem_statistics = DataInterface::getInstance().getProblemStatistics(problem_id);
  string databuf;
  databuf = stringPrintf("%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d",
                          problem_statistics.AC,
                          problem_statistics.Total,
                          problem_statistics.User_Total,
                          problem_statistics.User_AC,
                          problem_statistics.PE,
                          problem_statistics.TLE,
                          problem_statistics.MLE,
                          problem_statistics.OLE,
                          problem_statistics.WA,
                          problem_statistics.RE,
                          problem_statistics.CE);
  
  
  string len = stringPrintf("%010d",databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process ProblemStatistics completed for" << ip;
}

