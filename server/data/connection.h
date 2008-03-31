#ifndef _FLOOD_TEAM_CONNECTION_
#define _FLOOD_TEAM_CONNECTION_

#include <iostream>
#include <string>
#include "result.h"
using namespace std;

class Connection{
public:
	Connection() {}
  virtual ~Connection() {}
	
  virtual int connect();
	virtual int close();
	virtual int excuteUpdate(const string& query);
	virtual Result excuteQuery(const string& query);
};


#endif
