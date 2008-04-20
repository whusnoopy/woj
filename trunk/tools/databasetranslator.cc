#include <string>
#include <iostream>

#include "server/data/result.h"
#include "server/data/connection.h"
#include "server/data/databaseinterface.h"
#include "server/object/objectinc.h"

using namespace std;

Connection* s_connection;
Connection* d_connection;

Connection* createConnection(const string& host, 
                            const string& user, 
                            const string& password,
                            const string& name) {
  return new MysqlConnection(host, user, password, name);
}

string uncoding(const string& code) {
  string str;
  return str;
}

int getUser() {
  int i = 0;
  while (1) {
    string squery = "select * from user limit ";
    squery += stringPrintf("%d, %d", i*25, 25);
    i++;
    Result result_source = s_connection->excuteQuery(squery);
    if (result_source.getRowNum() == 0) {
      result_source.close();
      break;
    }
    while (result_source.next()) {
      User user;
      user.setId(result_source.getString("user_id"));
      user.setEmail(result_source.getString("email"));
      user.setShowEmail(true);
      user.setSolved(result_source.getInt("solved"));
      user.setSubmit(result_source.getInt("submit"));
      user.setAvailable(result_source.getInt("defunct") == 0 ? "N" : "Y");
      user.setLastLoginIp(result_source.getString("ip"));
      user.setLastLoginTime(result_source.getString("accesstime"));
      user.setVolume(result_source.getInt("volume"));
      user.setLanguage(result_source.getInt("language"));
      user.setPassword(uncoding(result_source.getString("pass")));
      user.setRegTime(result_source.getString("reg_time"));
      user.setNickname(result_source.getString("nick"));
      user.setSchool(result_source.getString("school"));
      user.setShareCode(false);
      user.setIndentifyCode("");
      ussr.setPermission(0);
      DatabaseInterface::getInstance().addUser(user);
    }
    result_source.close();
  }
}

int setUserPermission() {
  string squery = "select * from privilege";
  Result result_source = s_connection->excuteQuery(squery);
  while (result_source.next()) {
    User user = DatabaseInterface::getInstance().getUserInfo(result_source.getString("user_id"));
    string permission = result_source.getString("rigtstr");
    if (permission == "admin") {
      user.setPermission(user.getPermission() | 0x02);
    } else if (permission == "viewsource") {   
      user.setPermission(user.getPermission() | 0x01);
    }
    DatabaseInterface::getInstance().updateUserPermission(user); 
  }
  result_source.close();
}

int getProblem() {  
  int i = 0;
  while (1) {
    string squery = "select * from problem limit ";
    squery += stringPrintf("%d, %d", i*25, 25);
    i++;
    Result result_source = s_connection->excuteQuery(squery);
    if (result_source.getRowNum() == 0) {
      result_source.close();
      break;
    }
    while (result_source.next()) {
      string dquery = "insert into problems values(";
      problem_id;
      title;
      description;
      input;
      output;
      sample_input;
      sample_output;
      hint;
      source;
      addin_time;
      time_limit;
      case_time_limit;
      memory_limit;
      available;
      accepted;
      submit;
      solved_users;
      submit_users;
      standard_time_limit;
      standard_memory_limit;
      version;
      spj;
    }
    result_source.close();
  }
}

int getContest() {
  int i = 0;
  while (1) {
    string squery = "select * from contest limit ";
    squery += stringPrintf("%d, %d", i*25, 25);
    i++;
    Result result_source = s_connection->excuteQuery(squery);
    if (result_source.getRowNum() == 0) {
      result_source.close();
      break;
    }
    while (result_source.next()) {
      string dquery = "insert into contests values(";
      contest_id;
      public_id;
      title;
      start_time;
      end_time;
      contest_type;
      description;
      version;
      available;
    }
    result_source.close();
  }
}

int getProblemFile() {

}

int getContestFile() {

}

int getContestProblem() {

}

int getMails() {
  int i = 0;
  while (1) {
    string squery = "select * from mail";
    squery += stringPrintf("%d, %d", i*25, 25);
    i++;
    Result result_source = s_connection->excuteQuery(squery);
    if (result_source.getRowNum() == 0) {
      result_source.close();
      break;
    }
    while (result_source.next()) {
      string squery = "insert into mails values(";
      squery += "'" + string("%d", result_source.getInt("mail_id")) + "', ";
      squery += "'" + string("%d", result_source.getInt("reply")) + "', ";
      squery += "'" + changSymbol(result_source.getString("title")) + "', ";
      squery += "'" + changSymbol(result_source.getString("content")) + "', ";
      squery += "'" + (result_source.getInt("new_mail") == 1 ? "Y" : "N") + "', ";
      squery += "'" + result_source.getString("time") + "', ";
      squery += "'" + changeSymbol(result_source.getString("to_user")) + "', ";
      squery += "'" + changeSymbol(result_source.getString("from_user")) + "', ";
      if (result_source.getInt("defunct") == 1)
        squery += "'N', 'N')";
      else 
        squery += "'Y', 'Y')";
      d_connection->excuteUpdate(query);
    }
    result_source.close();
  } 
}

int getDiscuss() {
  int i = 0;
  while (1) {
    string squery = "select * from discuss limit ";
    squery += stringPrintf("%d, %d", i*25, 25);
    i++;
    Result result_source = s_connection->excuteQuery(squery);
    if (result_source.getRowNum() == 0) {
      result_source.close();
      break;
    }
    while (result_source.next()) {
      string dquery = "insert into discussess values(";
      message_id;
      reply_id;
      topic_id;
      user_id;
      problem_id;
      contest_id;
      title;
      content;
      time;
      available;
    }
    result_source.close();
  }
}

int getSolution() {
  
}

int main() {
  s_connection = createConnection();
  d_connection = createConnection();
  s_connection->connect();
  d_connection->connect();
  s_connection->close();
  d_connection->close();
  return 0;
}

