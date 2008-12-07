// Copyright 2008 Flood Team of Wuhan Univ.
// Author : ooeyusea@gmail.com (Qian, LIU)
// Modified : yewen@mail.whu.edu.cn (Wen, YE)

#include "server/data/mysqlconnection.h"

MysqlConnection::MysqlConnection(const string& host,
                                 const string& user,
                                 const string& password,
                                 const string& database)
: m_host(host),
  m_user(user),
  m_password(password),
  m_database(database)
{
}

int MysqlConnection::connect(){
  mysql_init(&m_mysql);
  if (mysql_real_connect(&m_mysql,
                         m_host.c_str(),
                         m_user.c_str(),
                         m_password.c_str(),
                         m_database.c_str(),
                         0,
                         NULL,
                         0))
	  return 1;
	else 
		return 0;
}

int MysqlConnection::close(){
  mysql_close(&m_mysql);
  return 0;
}

int MysqlConnection::excuteUpdate(const string& query){
  return mysql_real_query(&m_mysql,query.c_str(),query.length());
}

Result MysqlConnection::excuteQuery(const string& query){
	//cout << query << endl;
  int ret = mysql_real_query(&m_mysql,query.c_str(),query.length());
  //cout << ret <<endl;
  if(ret){
    cout << "Query failed." << query << endl;
    return Result();
  }else{
    m_result = mysql_store_result(&m_mysql);
    return Result(m_result);
  }
}

