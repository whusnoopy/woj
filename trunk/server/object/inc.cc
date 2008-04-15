#include "inc.h"
#include <iostream>
using namespace std;

DECLARE_FLAGS(string, root_dir);

string changeSymbol(const string& str){
  string strbuf(str);
  string seq = "'\\\"";
  LOG(DEBUG) << str;
  string::size_type pos = 0;
  while(( pos = strbuf.find_first_of(seq,pos)) != string::npos){
  	strbuf.insert(pos, 1, '\\');
  	pos += 2;
  	LOG(DEBUG) << "pos:" << pos;
  	LOG(DEBUG) << "strbuf: " << strbuf;
  } 
  return strbuf;
}

time_t changeTime(const string& time){
  struct tm time_buf;
  time_buf.tm_year = atoi(time.substr(0,4).c_str()) - 1900;
  time_buf.tm_mon = atoi(time.substr(5,2).c_str()) - 1;
  time_buf.tm_mday = atoi(time.substr(8,2).c_str());
  time_buf.tm_hour = atoi(time.substr(11,2).c_str());
  time_buf.tm_min = atoi(time.substr(14,2).c_str());
  time_buf.tm_sec = atoi(time.substr(17,2).c_str());
  return mktime(&time_buf);
}

int caltime(const string& end_time, const string& start_time){
  double diff = difftime(changeTime(end_time), changeTime(start_time));
  return static_cast<int> (diff);
}

string getProblemDataPath(int problem_id) {
  return FLAGS_root_dir + stringPrintf("/file/data/p%d/", problem_id);
}

string getContestDataPath(int contest_id) {
  return FLAGS_root_dir + stringPrintf("/file/data/p%d/", contest_id);
}

