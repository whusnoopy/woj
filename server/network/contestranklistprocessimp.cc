#include "contestranklistprocessimp.h"

#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "../object/info.h"
#include "../object/list.h"
#include "../util/calulate.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

string changeTime(int penalty){
  return stringPrintf("%02d:%02d:%02d", penalty/3600, (penalty%3600)/60,penalty%60);
}

bool isShorter(const ContestProblemTime& a, const ContestProblemTime& b) {
  return a.in_contest_id < b.in_contest_id;
}

vector<ContestProblemTime> changMapToVector(const map<int, ContestProblemTime>& time_map) {
  map<int, ContestProblemTime>::const_iterator problem_time_iter = time_map.begin();
  vector<ContestProblemTime> problem_time_list;
  ContestProblemTime problem_time;
  while (problem_time_iter != time_map.end()) {
    problem_time = problem_time_iter->second;
    problem_time_list.push_back(problem_time);
    problem_time_iter++;
  }
  stable_sort(problem_time_list.begin(), problem_time_list.end(), isShorter);
  return problem_time_list; 
}

void ContestRankListProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process Contest RankList for:" << ip;
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
  
  ContestRankListInfo contest_ranklist_info;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find contest_id from data for:" << ip;
    return;
  }
  contest_ranklist_info.contest_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find page_id from data for:" << ip;
    return;
  }
  contest_ranklist_info.page_id = atoi(iter->c_str()); 

  ContestRankList contest_ranklist;
  int contest_problem_num = 0;
  //contest_problem_num = DataInterface::getInstance().getContestProblemNum(contest_ranklist_info.contest_id);
  //contest_ranklist = DataInterface::getInstance().getContestRankList(contest_ranklist_info);
  //should sort the seq in DataInterface;
  string databuf;
  databuf += stringPrintf("%d", contest_problem_num);
  ContestRankList::iterator ranklist_iter = contest_ranklist.begin();
  while (ranklist_iter != contest_ranklist.end()) {
    databuf += stringPrintf("\001%s\001%s\001%d",
                            ranklist_iter->user_id.c_str(),
                            ranklist_iter->nickname.c_str(),
                            ranklist_iter->accepted);
    databuf += "\001" + changeTime(ranklist_iter->penalty);
    //sort
    vector<ContestProblemTime> problem_time_list = changMapToVector(ranklist_iter->problem_penalty);
    vector<ContestProblemTime>::iterator problem_time_iter = problem_time_list.begin();
    while (problem_time_iter != problem_time_list.end()) {
      databuf += "\001" + changeTime(problem_time_iter->penalty);
      databuf += stringPrintf("\001%d", -(problem_time_iter->submit-1));
      problem_time_iter++;
    }

    ranklist_iter++;
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
  LOG(INFO) << "Process ContestRankList completed for" << ip;
}

