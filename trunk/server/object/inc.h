// Copyright 2008 Flood Team of Wuhan Univ.

#ifndef _FLOOD_SERVER_DATA_INC_H__
#define _FLOOD_SERVER_DATA_INC_H__

#include <string>

using namespace std;

string changeSymbol(const string& str);

time_t changeTime(const string& time);

int caltime(const string& end_time, const string& start_time);

string getProblemDataPath(int problem_id);

string getContestDataPath(int contest_id); 

#endif // end of _FLOOD_SERVER_DATA_INC_H__

