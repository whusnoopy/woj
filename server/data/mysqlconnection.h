// Copyright 2008 Flood Team of Wuhan Univ.
// Author : ooeyusea@gmail.com (Qian, LIU)
// Modified : yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _FLOOD_TEAM_MYSQL_CONNECTION_
#define _FLOOD_TEAM_MYSQL_CONNECTION_

#include <iostream>
#include <string>

#include <mysql.h>

#include "server/data/connection.h"
#include "server/data/result.h"

using namespace std;

class MysqlConnection : public Connection{
public:
	MysqlConnection() {}
	MysqlConnection(const string& host,
			            const string& user,
									const string& password,
									const string& database);
	int connect();
	int close();
	int excuteUpdate(const string& query);
	Result excuteQuery(const string& query);
private:
	MYSQL m_mysql;
	MYSQL_RES * m_result;
	string m_host,m_user,m_password,m_database;
};

#endif
