#include <string>
#include <fstream>
#include <iostream>

#include <fcntl.h>

#include "./data/result.h"
#include "./data/connection.h"
#include "./data/databaseinterface.h"
#include "./object/objectinc.h"
#include "../base/judge_result.h"
#include "../base/util.h"
#include "md5.h"

using namespace std;

Connection* s_connection;
Connection* d_connection;

Connection* createConnection(const string& host, 
                            const string& user, 
                            const string& password,
                            const string& name) {
  return new MysqlConnection(host, user, password, name);
}

string encoding(const string& code) {
  char * temp = new char[code.length() +10];
  memset(temp, 0, code.length() + 10);
  memcpy(temp, code.c_str(), code.length());
  return MDString(temp);
}

int resultchange(int result) {
  switch (result) {
    case 0:
      return ACCEPTED;
    case 1:
      return PRESENTATION_ERROR;
    case 2:
      return TIME_LIMIT_EXCEEDED;
    case 3:
      return MEMORY_LIMIT_EXCEEDED;
    case 4:
      return WRONG_ANSWER;
    case 5:
      return OUTPUT_LIMIT_EXCEEDED;
    case 6:
      return COMPILE_ERROR;
    case 7:
      return RUNTIME_ERROR_SIGSEGV;
    case 8:
      return RUNTIME_ERROR_SIGFPE;
    case 9:
      return RUNTIME_ERROR_SIGBUS;
    case 10:
      return RUNTIME_ERROR_SIGABRT;
    case 11:
      return RUNTIME_ERROR_JAVA;
    case 12:
      return RUNTIME_ERROR_SIGSEGV;
    case 13:
      return RESTRICTED_FUNCTION;
    case 14:
      return SYSTEM_ERROR;
  }
  return 0;
}

int languagechange(int language) {
  switch (language) {
    case 0:
      return GCC;
    case 1:
      return GPP;
    case 2:
      return JAVA;
    case 3:
      return PASCAL;
  }
  return language; 
}

int copyfile(const string& source_file, const string& destiny_file) {
  string destiny_dir = destiny_file.substr(0, destiny_file.find_last_of("/"));
  if (mkdirRecursive(destiny_dir, 0755) < 0)
    return -1;
  int out_fd = open(destiny_file.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0644);
  close(out_fd);
  ifstream in(source_file.c_str(), ios::binary);
  ofstream out(destiny_file.c_str(), ios::binary);
  out << in.rdbuf();
  in.close();
  out.close();
  return 0;
}

int getUser() {
  int i = 0;
  while (1) {
    string squery = "select user_id, email, solved, submit, defunct, "
                    "ip, accesstime, volume, language, AES_DECRYPT(pass, 'noah') as password, "
                    "reg_time, nick,school from user limit ";
    squery += stringPrintf("%d, %d", i*25, 25);
    cout << squery << endl;
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
      user.setAvailable(result_source.getString("defunct") == "N");
      user.setLastLoginIp(result_source.getString("ip"));
      user.setLastLoginTime(result_source.getString("accesstime"));
      user.setVolume(result_source.getInt("volume"));
      user.setLanguage(result_source.getInt("language"));
      user.setPassword(encoding(result_source.getString("password")));
      user.setRegTime(result_source.getString("reg_time"));
      user.setNickname(result_source.getString("nick"));
      user.setSchool(result_source.getString("school"));
      user.setShareCode(false);
      user.setIndentifyCode("");
      user.setPermission(0);

      string dquery = "insert into users(user_id, email, show_email, submits, solveds, ";
      dquery += "available, last_login_ip, last_login_time, volume, language, ";
      dquery += "password, reg_time, nickname, school, permission, share_code, ";
      dquery += "indentify_code) values('"; 
      dquery += changeSymbol(user.getId()) + "','" +
                changeSymbol(user.getEmail()) + "','" +
                changeSymbol(user.getShowEmail()?"Y":"N") + "','" +
                stringPrintf("%d",user.getSubmit()) + "','" +
                stringPrintf("%d",user.getSolved()) + "','" +
                changeSymbol(user.getAvailable()?"Y":"N") + "','" +
                changeSymbol(user.getLastLoginIp()) + "','" +
                changeSymbol(user.getLastLoginTime()) + "','" +
                stringPrintf("%d",user.getVolume()) + "','" +
                stringPrintf("%d",user.getLanguage()) + "','" +
                changeSymbol(user.getPassword()) + "','" +
                changeSymbol(user.getRegTime()) + "','" +
                changeSymbol(user.getNickname()) + "','" +
                changeSymbol(user.getSchool()) + "','";
      string str="----";
      if (user.getPermission() & 0x02 )
        str += "A";
      else
        str += "-";
      if (user.getPermission() & 0x01 )
        str += "V";
      else
        str += "-";
      dquery += changeSymbol(str) + "','";
      dquery += changeSymbol(user.getShareCode()?"Y":"N") + "','" +
               changeSymbol(user.getIndentifyCode()) + 
               "')";
      cout << dquery <<endl;
      d_connection->excuteUpdate(dquery);
    }
    result_source.close();
  }
  return 0;
}

int setUserPermission() {
  string squery = "select * from privilege";
  Result result_source = s_connection->excuteQuery(squery);
  while (result_source.next()) {
    string user_id = result_source.getString("user_id"); 
    string dquery = "select * from users where user_id = '" + changeSymbol(user_id) + "'";
    Result result_des = d_connection->excuteQuery(dquery);
    string per = "------";
    if (result_des.next()) {
      per = result_des.getString("permission");
    }
    result_des.close();
    string permission = result_source.getString("rightstr");
    if (permission == "admin") {
      per[4] = 'A';
    } else if (permission == "viewsource") {   
      per[5] = 'V';
    }
    dquery = "update users set permission = '" + changeSymbol(per) + "' ";
    dquery += "where user_id = '" + changeSymbol(user_id) + "'";
    d_connection->excuteUpdate(dquery);
  }
  result_source.close();
  return 0;
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
      int problem_id = result_source.getInt("problem_id");
      dquery += "'" + stringPrintf("%d", result_source.getInt("problem_id")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("title")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("description")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("input")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("output")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("sample_input")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("sample_output")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("hint")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("source")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("in_date")) + "', ";
      dquery += "'" + stringPrintf("%d", result_source.getInt("time_limit")) + "', ";
      dquery += "'" + stringPrintf("%d", result_source.getInt("case_time_limit")) + "', ";
      dquery += "'" + stringPrintf("%d", result_source.getInt("memory_limit")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("defunct") == "N" ? "Y" : "N") + "', ";
      dquery += "'" + stringPrintf("%d", result_source.getInt("accepted")) + "', ";
      dquery += "'" + stringPrintf("%d", result_source.getInt("submit")) + "', ";
      dquery += "'" + stringPrintf("%d", result_source.getInt("solved")) + "', ";
      dquery += "'" + stringPrintf("%d", result_source.getInt("submit_user")) + "', ";
      dquery += "'0', ";
      dquery += "'0', ";
      dquery += "'1', ";
      string path = "/home/flood/origSerBak/data/" + stringPrintf("%d/spj_judge", problem_id);
      if (access(path.c_str(), R_OK) < 0) 
        dquery += "'N'";
      else
        dquery += "'Y'";
      dquery += ")";
      d_connection->excuteUpdate(dquery);
    }
    result_source.close();
  }
  return 0;
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
      dquery += "'" + stringPrintf("%d", result_source.getInt("contest_id")) + "', ";
      dquery += "'" + stringPrintf("%d", result_source.getInt("contest_id")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("title")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("start_time")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("end_time")) + "', ";
      dquery += "'N', ";
      dquery += "'" + changeSymbol(result_source.getString("description")) + "', ";
      dquery += "'1', ";
      dquery += "'" + changeSymbol(result_source.getString("defunct") == "N" ? "Y" : "N") + "')";
      d_connection->excuteUpdate(dquery);
    }
    result_source.close();
  }
  return 0;
}

int getProblemFile() {
   string squery = "select problem_id from problem";
   Result result_source = s_connection->excuteQuery(squery);
   while (result_source.next()) {
     int problem_id = result_source.getInt("problem_id");
     string dir = "/home/flood/origSerBak/data/" + stringPrintf("%d/", problem_id);
     string path = dir + "data.txt";
     ifstream in(path.c_str());
     string line;
     string dquery;
     int i = 0;
     cout << "path:" << path << endl;
     while (getline(in, line)) {
       string nosuffix = line.substr(0, line.find_last_of("."));
       cout << "line:" << line << endl;
       dquery = "insert into files(path, style) values(";
       string infilepath = "/home/flood/worktemp/file/data/p" + stringPrintf("%d/%d.in", problem_id, i);
       dquery += "'" + changeSymbol(infilepath) + "', ";
       dquery += " '1')";
       d_connection->excuteUpdate(dquery);
       dquery = "select LAST_INSERT_ID() as file_id";
       Result result_d = d_connection->excuteQuery(dquery);
       int in_id = 0;
       if (result_d.next()) 
         in_id = result_d.getInt("file_id");
       result_d.close();
       if (in_id == 0) {
         LOG(ERROR) << "in_id error" << infilepath;
         return -1;
       }
       dquery = "insert into filestoproblems values(";
       dquery += stringPrintf("'%d', '%d', '1')", in_id, problem_id);
       d_connection->excuteUpdate(dquery);
       copyfile(dir + nosuffix + ".in", infilepath);

       dquery = "insert into files(path, style) values(";
       string outfilepath = "/home/flood/worktemp/file/data/p" + stringPrintf("%d/%d.out", problem_id, i++);
       dquery += "'" + changeSymbol(outfilepath) + "', ";
       dquery += " '2')";
       d_connection->excuteUpdate(dquery);
       dquery = "select LAST_INSERT_ID() as file_id";
       result_d = d_connection->excuteQuery(dquery);
       int out_id = 0;
       if (result_d.next()) 
         out_id = result_d.getInt("file_id");
       result_d.close();
       if (out_id == 0) {
         LOG(ERROR) << "out_id error" << outfilepath;
         return -1;
       }
       dquery = "insert into filestoproblems values(";
       dquery += stringPrintf("'%d', '%d', '1')", out_id, problem_id);
       d_connection->excuteUpdate(dquery);

       dquery = "insert into intooutfiles values(";
       dquery += stringPrintf("'%d', '%d')", in_id, out_id);
       d_connection->excuteUpdate(dquery); 
       copyfile(dir + nosuffix + ".out", outfilepath);
      
     }
     in.close();
     string spj = dir + "spj_judge";
     if (access(spj.c_str(), R_OK) == 0) {       
       dquery = "insert into files(path, style) values(";
       string spjfilepath = "/home/flood/worktemp/file/data/p" + stringPrintf("%d/spj.cc", problem_id);
       dquery += "'" + changeSymbol(spjfilepath) + "', ";
       dquery += " '4')";
       d_connection->excuteUpdate(dquery);
       dquery = "select LAST_INSERT_ID() as file_id";
       Result result_d = d_connection->excuteQuery(dquery);
       int spj_id = 0;
       if (result_d.next()) 
         spj_id = result_d.getInt("file_id");
       result_d.close();
       if (spj_id == 0) {
         LOG(ERROR) << "spj_id error" << spjfilepath;
         return -1;
       }
       dquery = "insert into filestoproblems values(";
       dquery += stringPrintf("'%d', '%d', '1')", spj_id, problem_id);
       d_connection->excuteUpdate(dquery);
       copyfile(spj + ".cc", spjfilepath); 
     }
   }
   return 0;
}

int getContestFile() {
  return 0;
}

int getContestProblem() {
  string squery = "select contest_id from contest";
  Result result_s = s_connection->excuteQuery(squery);
  while (result_s.next()) {
    squery = "select * from contest_problem where contest_id = ";
    squery += stringPrintf("'%d' ", result_s.getInt("contest_id"));
    squery += " order by num";
    Result result_source = s_connection->excuteQuery(squery);
    int i = 1;
    while (result_source.next()) {
      string dquery = "insert into problemtocontests values(";
      dquery += stringPrintf("'%d', '%d', '%d')", 
                result_source.getInt("problem_id"), 
                result_source.getInt("contest_id"), i++); 
      d_connection->excuteUpdate(dquery);
    }
    result_source.close();
  }
  result_s.close();
  return 0;
}

int getMails() {
  int i = 0;
  int cnt = 0;
  while (1) {
    string squery = "select * from mail";
    squery += stringPrintf(" limit %d, %d", i*25, 25);
    i++;
    Result result_source = s_connection->excuteQuery(squery);
    if (result_source.getRowNum() == 0) {
      result_source.close();
      break;
    }
    while (result_source.next()) {
      string dquery = "insert into mails values(";
      dquery += "'" + stringPrintf("%d", result_source.getInt("mail_id")) + "', ";
      dquery += "'" + stringPrintf("%d", result_source.getInt("reply")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("title")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("content")) + "', ";
      dquery += "'" + changeSymbol(result_source.getInt("new_mail") == 1 ? "Y" : "N") + "', ";
      dquery += "'" + result_source.getString("in_date") + "', ";
      dquery += "'" + changeSymbol(result_source.getString("to_user")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("from_user")) + "', ";
      if (result_source.getString("defunct") == "N")
        dquery += "'N', 'N')";
      else 
        dquery += "'Y', 'Y')";
      if (++cnt == 1)
        cout << "dquery:" << dquery << endl;
      d_connection->excuteUpdate(dquery);
    }
    result_source.close();
  } 
  return 0;
}

int getDiscuss() {
  int i = 0;
  int cnt = 0;
  while (1) {
    string squery = "select * from message limit ";
    squery += stringPrintf("%d, %d", i*25, 25);
    cout << "squery:" << squery << endl;
    i++;
    Result result_source = s_connection->excuteQuery(squery);
    if (result_source.getRowNum() == 0) {
      result_source.close();
      break;
    }
    while (result_source.next()) {
      string dquery = "insert into discusses values(";
      dquery += stringPrintf("'%d',", result_source.getInt("message_id"));
      dquery += stringPrintf("'%d',", result_source.getInt("parent_id"));
      dquery += stringPrintf("'%d',", -1);
      dquery += "'" + changeSymbol(result_source.getString("user_id")) + "', ";
      dquery += stringPrintf("'%d',", result_source.getInt("problem_id"));
      dquery += "'0', ";
      dquery += "'" + changeSymbol(result_source.getString("title")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("content")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("in_date")) + "', ";
      dquery += "'" + changeSymbol(result_source.getString("defunct") == "N" ? "Y" : "N") + "') ";
      if (++cnt = 1) 
        cout << "dquery : " << dquery << endl;
      d_connection->excuteUpdate(dquery);
    }
    result_source.close();
  }
  return 0;
}

int getChildTree(int father, int topic_id) {
  string dquery = "select * from discusses where reply_id = '" + stringPrintf("%d'", father);
  Result result_source = d_connection->excuteQuery(dquery);
  vector<int> discuss_list;
  while (result_source.next()) {
    discuss_list.push_back(result_source.getInt("message_id"));
  }
  result_source.close();
  vector<int>::iterator iter = discuss_list.begin();
  while (iter != discuss_list.end()) {
    dquery = "update discusses set topic_id = " + stringPrintf("'%d'", topic_id) + " where message_id = '" + stringPrintf("%d'", *iter);
    d_connection->excuteUpdate(dquery);
    getChildTree(*iter, topic_id);
    iter++;
  }
  return 0;
}

int getDiscussTopicId() {
  string dquery = "select * from discusses where reply_id = '0'";
  Result result_source = d_connection->excuteQuery(dquery);
  vector<int> discuss_list;
  while (result_source.next()) {
    discuss_list.push_back(result_source.getInt("message_id"));
  }
  result_source.close();
  vector<int>::iterator iter = discuss_list.begin();
  while (iter != discuss_list.end()) {
    dquery = "update discusses set topic_id = message_id where message_id = '" + stringPrintf("%d'", *iter);
    d_connection->excuteUpdate(dquery);
    getChildTree(*iter, *iter);
    iter++;
  }
  dquery = "update discusses set topic_id = message_id where topic_id = '-1'";
  d_connection->excuteUpdate(dquery);
  return 0;
}

int getSolution() { 
  int i = 0;
  while (1) {
    string squery = "select * from solution limit ";
    squery += stringPrintf("%d, %d", i*1000, 1000);
    i++;
    Result result_source = s_connection->excuteQuery(squery);
    if (result_source.getRowNum() == 0) {
      result_source.close();
      break;
    }
    while (result_source.next()) {
      int solution_id = result_source.getInt("solution_id");
      int problem_id = result_source.getInt("problem_id");
      string user_id = result_source.getString("user_id");
      int time = result_source.getInt("time");
      int memory = result_source.getInt("memory");
      string in_date = result_source.getString("in_date");
      int result = result_source.getInt("result");
      int language = result_source.getInt("language");
      string ip = result_source.getString("ip");
      int contest_id = result_source.getInt("contest_id");
      int code_length = result_source.getInt("code_length");
      int code_id = 0;
      string source;
      int error_id = 0;
      string error_info;
      
      squery = "select source from source_code where solution_id = ";
      squery += stringPrintf("'%d'", solution_id);
      Result result_s = s_connection->excuteQuery(squery);
      if (result_s.next()) 
        source = result_s.getString("source");
      result_s.close();
      string dquery = "insert into codes(share, code_content) values (";
      dquery += "'N', ";
      dquery += "'" + changeSymbol(source) + "') ";
      d_connection->excuteUpdate(dquery);
      dquery = "select Last_insert_id() as code_id";
      Result result_d = d_connection->excuteQuery(dquery);
      if (result_d.next())
        code_id = result_d.getInt("code_id");
      result_d.close();

      squery = "select error from compileinfo where solution_id = ";
      squery += stringPrintf("'%d'", solution_id);
      result_s = s_connection->excuteQuery(squery);
      if (result_s.next()) 
        error_info = result_s.getString("error");
      result_s.close();
      
      squery = "select reerror as error from pascalreinfo where solution_id = ";
      squery += stringPrintf("'%d'", solution_id);
      result_s = s_connection->excuteQuery(squery);
      if (result_s.next()) 
        error_info = result_s.getString("error");
      result_s.close();

      squery = "select reerror as error from javareinfo where solution_id = ";
      squery += stringPrintf("'%d'", solution_id);
      result_s = s_connection->excuteQuery(squery);
      if (result_s.next()) 
        error_info = result_s.getString("error");
      result_s.close();
     
      if (!error_info.empty()) {
        dquery = "insert into errors(content) values (";
        dquery += "'" + changeSymbol(error_info) + "') ";
        d_connection->excuteUpdate(dquery);
        dquery = "select Last_insert_id() as error_id";
        result_d = d_connection->excuteQuery(dquery);
        if (result_d.next())
          error_id = result_d.getInt("error_id");
        result_d.close();
      }

      dquery = "insert into statuses values(";
      dquery += "'" + stringPrintf("%d", solution_id)  + "', ";
      dquery += "'" + changeSymbol(user_id) + "', ";
      dquery += "'" + stringPrintf("%d", problem_id)  + "', ";
      dquery += "'" + stringPrintf("%d", contest_id)  + "', ";
      dquery += "'" + stringPrintf("%d", time)  + "', ";
      dquery += "'" + stringPrintf("%d", memory)  + "', ";
      dquery += "'" + changeSymbol(in_date) + "', ";
      dquery += "'" + stringPrintf("%d", resultchange(result))  + "', ";
      dquery += "'" + stringPrintf("%d", languagechange(language))  + "', ";
      dquery += "'" + stringPrintf("%d", code_id)  + "', ";
      dquery += "'" + stringPrintf("%d", code_length)  + "', ";
      dquery += "'" + changeSymbol(ip) + "', ";
      dquery += "'" + stringPrintf("%d", error_id)  + "', ";
      dquery += "'N')";
      d_connection->excuteUpdate(dquery);
    }
    result_source.close();
  }
  return 0;
}

int getRootSolution() { 
  int i = 0;
  while (1) {
    string squery = "select * from rootsolution limit ";
    squery += stringPrintf("%d, %d", i*200, 200);
    i++;
    Result result_source = s_connection->excuteQuery(squery);
    if (result_source.getRowNum() == 0) {
      result_source.close();
      break;
    }
    while (result_source.next()) {
      int solution_id = result_source.getInt("solution_id");
      int problem_id = result_source.getInt("problem_id");
      string user_id = result_source.getString("user_id");
      int time = result_source.getInt("time");
      int memory = result_source.getInt("memory");
      string in_date = result_source.getString("in_date");
      int result = result_source.getInt("result");
      int language = result_source.getInt("language");
      string ip = result_source.getString("ip");
      int contest_id = result_source.getInt("contest_id");
      int code_length = result_source.getInt("code_length");
      int code_id = 0;
      string source;
      int error_id = 0;
      string error_info;

      squery = "select source from rootsource_code where solution_id = ";
      squery += stringPrintf("'%d'", solution_id);
      Result result_s = s_connection->excuteQuery(squery);
      if (result_s.next()) 
        source = result_s.getString("source");
      result_s.close();
      string dquery = "insert into codes(share, code_content) values (";
      dquery += "'N', ";
      dquery += "'" + changeSymbol(source) + "') ";
      d_connection->excuteUpdate(dquery);
      dquery = "select Last_insert_id() as code_id";
      Result result_d = d_connection->excuteQuery(dquery);
      if (result_d.next())
        code_id = result_d.getInt("code_id");
      result_d.close();

      dquery = "insert into statuses(user_id, problem_id, contest_id, time,";
      dquery += " memory, submit_time, result, language, code_id, code_length, submit_ip, error_id, type)  values(";
      dquery += "'" + changeSymbol(user_id) + "', ";
      dquery += "'" + stringPrintf("%d", problem_id)  + "', ";
      dquery += "'" + stringPrintf("%d", contest_id)  + "', ";
      dquery += "'" + stringPrintf("%d", time)  + "', ";
      dquery += "'" + stringPrintf("%d", memory)  + "', ";
      dquery += "'" + changeSymbol(in_date) + "', ";
      dquery += "'" + stringPrintf("%d", resultchange(result))  + "', ";
      dquery += "'" + stringPrintf("%d", languagechange(language))  + "', ";
      dquery += "'" + stringPrintf("%d", code_id)  + "', ";
      dquery += "'" + stringPrintf("%d", code_length)  + "', ";
      dquery += "'" + changeSymbol(ip) + "', ";
      dquery += "'" + stringPrintf("%d", error_id)  + "', ";
      dquery += "'R')";
      d_connection->excuteUpdate(dquery);
    }
    result_source.close();
  }
  return 0;
}

int main() {
  s_connection = createConnection("localhost", "root", "123456", "noah");
  d_connection = createConnection("localhost", "root", "123456", "onlinejudgetest");
  s_connection->connect();
  d_connection->connect();
  getUser();
  setUserPermission();
  getProblem();
  getContest();
  getProblemFile();
  getContestProblem();
  getMails();
  getDiscuss();
  getDiscussTopicId();
  getSolution();
  getRootSolution();
  s_connection->close();
  d_connection->close();
  return 0;
}

