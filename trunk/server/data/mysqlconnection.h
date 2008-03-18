#ifndef _FLOOD_TEAM_MYSQL_CONNECTION_
#define _FLOOD_TEAM_MYSQL_CONNECTION_

#include <iostream>
#include <string>
using namespace std;

#include <mysql.h>
#include "connection.h"
#include "result.h"

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
