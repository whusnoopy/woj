// Copyright 2008 Flood Team of Wuhan Univ.
// Author : ooeyusea@gmail.com (Qian, LIU)
// Modified : yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _FLOOD_TEAM_CONNECTION_
#define _FLOOD_TEAM_CONNECTION_

#include <iostream>
#include <string>
#include "server/data/result.h"

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
