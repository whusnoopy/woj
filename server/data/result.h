#ifndef _FLOOD_TEAM_RESULT_
#define _FLOOD_TEAM_RESULT_

#include <iostream>
#include <string>
#include <map>

#include <mysql.h>

using namespace std;

class Result {
public:
  Result(){}
  Result(MYSQL_RES* result);
	virtual ~Result();

	bool next();
	//bool previous();
	//void afterlast();
	//void beforefirst();
	int getRowNum();
	string getString(const string& name);
	int getInt(const string& name);
  int getInt(int n);
	//double getDouble(const string& name);
	void close();
private:
	MYSQL_RES* m_result;
	MYSQL_ROW m_row;
	map<string,int> m_name_map;
};

#endif
