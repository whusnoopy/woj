#ifndef _FLOOD_SERVER_DATA_INC_H__
#define _FLOOD_SERVER_DATA_INC_H__

#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"

using namespace std;

DEFINE_FLAGS(string, root_dir, "");

string changeSymbol(const string& str){
  string strbuf(str);
  string::size_type pos = 0;
  while(( pos = strbuf.find_first_of("_'\\\"%",pos)) != string::npos){
  	strbuf.insert(pos, 1, '\\');
  	pos += 2;
  	cout << "pos:" << pos << endl;
  	cout << "strbuf: " << strbuf <<endl;
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
#endif
