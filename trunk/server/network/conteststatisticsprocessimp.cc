#include "conteststatisticsprocessimp.h"

#include <string>
#include <vector>
#include <algorithm>

#include "object/info.h"
#include "object/list.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

bool isShorter(const ContestStatisticsItem& a, const ContestStatisticsItem& b) {
  return a.incontest_id < b.incontest_id;
}

void ContestStatisticsProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process Contest Statistics for:" << ip;
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
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find contest_id from data for:" << ip;
    return;
  }
  int contest_id = atoi(iter->c_str());
  ContestStatistics contest_statistics;
  contest_statistics = DataInterface::getInstance().getContestStatistics(contest_id);
  int problem_num = DataInterface::getInstance().getContestProblemNum(contest_id);
  string problem_num_str = stringPrintf("%010d", problem_num);
  if (socket_write(socket_fd, problem_num_str.c_str(), 10)) {
    LOG(ERROR) << "Cannot send problem number to" << ip;
    return ;
  }
  stable_sort(contest_statistics.begin(), contest_statistics.end(), isShorter);
  string databuf;
  ContestStatistics::iterator statistics_iter = contest_statistics.begin();
  bool first = true;
  while (statistics_iter != contest_statistics.end()) {
    if (first) {
      databuf += stringPrintf("%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d",
                              statistics_iter->problem_id,
                              statistics_iter->AC,
                              statistics_iter->PE,
                              statistics_iter->CE,
                              statistics_iter->WA,
                              statistics_iter->TLE,
                              statistics_iter->RE,
                              statistics_iter->MLE,
                              statistics_iter->OLE,
                              statistics_iter->Total,
                              statistics_iter->C_CPP,
                              statistics_iter->Java,
                              statistics_iter->Pascal);
      first = false;
    }else {
      databuf += stringPrintf("\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d\001%d",
                              statistics_iter->problem_id,
                              statistics_iter->AC,
                              statistics_iter->PE,
                              statistics_iter->CE,
                              statistics_iter->WA,
                              statistics_iter->TLE,
                              statistics_iter->RE,
                              statistics_iter->MLE,
                              statistics_iter->OLE,
                              statistics_iter->Total,
                              statistics_iter->C_CPP,
                              statistics_iter->Java,
                              statistics_iter->Pascal);
    }
    statistics_iter++;
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
  LOG(INFO) << "Process Contest Statistics completed for" << ip;
}

