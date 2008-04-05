#ifndef _FLOOD_SERVER_DATA_INC_H__
#define _FLOOD_SERVER_DATA_INC_H__

#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "base/judge_result.h"

using namespace std;

//DEFINE_FLAGS(string, root_dir, "");

string changeSymbol(const string& str);

time_t changeTime(const string& time);

int caltime(const string& end_time, const string& start_time);

string getProblemDataPath(int problem_id);

string getContestDataPath(int contest_id); 
#endif

