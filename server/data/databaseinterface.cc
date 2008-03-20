#include <iostream>

#include "databaseinterface.h"
#include "info.h"
#include "list.h"
#include "mysqlconnection.h"
#include "result.h"
#include "contest.h"
#include "news.h"
#include "problem.h"
#include "status.h"
#include "mail.h"
#include "user.h"
#include "code.h"
#include "file.h"
#include "error.h"
#include "discuss.h"
#include "inc.h"
using namespace std;

DatabaseInterface* DatabaseInterface::instance = NULL;

int DatabaseInterface::addContest(const Contest& contest){
  Connection* connection = createConnection("localhost",
                                           "root",
                                           "noahoak",
                                           "onlinejudgetest");
  string query;
  query += "insert into contests(public_id, title, description,"; 
  query +=" start_time, end_time, contest_type) values('";
  query += stringPrintf("%d",contest.getPublicId()) + "','" +
           contest.getTitle() + "','" +
           contest.getDescription() + "','" +
           contest.getStartTime() + "','" +
           contest.getEndTime() + "','" +
           contest.getType() + 
           "')";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::addProblemListtoContest(const Contest& contest,
                            const ProblemList& problem_list){
  Connection* connection = createConnection("localhost",
                                           "root",
                                           "noahoak",
                                           "onlinejudgetest");
  int in_contest_id = 1;
  ProblemList::const_iterator problem_iter = problem_list.begin();
  int ret;
  cout << "Connection:" << connection->connect() << endl;
  while(problem_iter != problem_list.end()){
    string query;
    query += "insert into problemtocontests(problem_id, contest_id, "; 
    query += "in_contest_id) values('";
    query += stringPrintf("%d", problem_iter->problem_id) + "','" +
             stringPrintf("%d", contest.getContestId()) + "','" +
             stringPrintf("%d", in_contest_id) + 
             "')";
    cout << query << endl;
    ret = connection->excuteUpdate(query);
    if (ret)
      cout << "Add Problem " << problem_iter->problem_id << "to Contest "
           << contest.getContestId() << endl;
    in_contest_id++;
    problem_iter++;
  }
  connection->close();
  delete connection;
  return ret;                             
}

int DatabaseInterface::addUserListtoContest(const Contest& contest,const UserList& user_list){
  Connection* connection = createConnection("localhost",
                                           "root",
                                           "noahoak",
                                           "onlinejudgetest");
  UserList::const_iterator user_iter = user_list.begin();
  int ret;
  cout << "Connection:" << connection->connect() << endl;
  while(user_iter != user_list.end()){
    string query;
    query += "insert into contestpermission(user_id, contest_id) values('";
    query += changeSymbol(user_iter->user_id) + "','" +
             stringPrintf("%d", contest.getContestId()) +
             "')";
    cout << query << endl;
    ret = connection->excuteUpdate(query);
    if (ret)
      cout << "Add User " << user_iter->user_id << "to Contest "
           << contest.getContestId() << endl;
    user_iter++;
  }
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::addNews(const News& news){
  Connection* connection = createConnection("localhost",
                                           "root",
                                           "noahoak",
                                           "onlinejudgetest");
  string query;
  query += "insert into news(publishtime, title, content) values('"; 
  query += changeSymbol(news.getPublishTime()) + "','" +
           changeSymbol(news.getTitle()) + "','" +
           changeSymbol(news.getContent()) + 
           "')";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection; 
  return ret;
}

/*
int addNotice(const Notice& notice);
*/
int DatabaseInterface::addProblem(const Problem& problem){
	Connection* connection = createConnection("localhost",
                                           "root",
                                           "noahoak",
                                           "onlinejudgetest");
  string query;
  query += "insert into problems(title, description, input, output, ";
  query += "sample_input, sample_output, hint, source, addin_time, ";
  query += "time_limit, case_time_limit, memory_limit, available, ";
  query += "accepted, submit, solved_users, submit_users, ";
  query += "standard_time_limit, standard_memory_limit, version, spj) values('"; 
  query += changeSymbol(problem.getTitle()) + "','" +
           changeSymbol(problem.getDescription()) + "','" +
           changeSymbol(problem.getInput()) + "','" +
           changeSymbol(problem.getOutput()) + "','" +
           changeSymbol(problem.getSampleInput()) + "','" +
           changeSymbol(problem.getSampleOutput()) + "','" +
           changeSymbol(problem.getHint()) + "','" +
           changeSymbol(problem.getSource()) + "','" +
           changeSymbol(problem.getAddinTime()) + "','" +
           stringPrintf("%d",problem.getTimeLimit()) + "','" +
           stringPrintf("%d",problem.getCaseTimeLimit()) + "','" +
           stringPrintf("%d",problem.getMemoryLimit()) + "','" +
           changeSymbol(problem.getAvailable()?"Y":"N") + "','" +
           stringPrintf("%d",problem.getAccepted()) + "','" +
           stringPrintf("%d",problem.getSubmit()) + "','" +
           stringPrintf("%d",problem.getSolvedUsers()) + "','" +
           stringPrintf("%d",problem.getSubmitUsers()) + "','" +
           stringPrintf("%d",problem.getStandardTimeLimit()) + "','" +
           stringPrintf("%d",problem.getStandardMemoryLimit()) + "','" +
           stringPrintf("%d",problem.getVersion()) + "','" +
           changeSymbol(problem.getSpj()?"Y":"N") + 
           "')";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection; 
  return ret;
}

int DatabaseInterface::addStatus(const Status& stautus){
  Connection* connection = createConnection("localhost",
                                           "root",
                                           "noahoak",
                                           "onlinejudgetest");
  string query;
  query += "insert into statuses(user_id, problem_id, contest_id, time, ";
  query += "memory, submit_time, result, language, code_id, code_length, ";
  query += "submit_ip, error_id) values('";
  query += changeSymbol(status.getUserId()) + "','" +
           stringPrintf("%d", status.getProblemId()) + "','" +
           stringPrintf("%d", status.getContestId()) + "','" +
           stringPrintf("%d", status.getTime()) + "','" +
           stringPrintf("%d", status.getMemory()) + "','" +
           changeSymbol(status.getSubmitTime()) + "','" +
           stringPrintf("%d", status.getResult()) + "','" +
           stringPrintf("%d", status.getLanguage()) + "','" +
           stringPrintf("%d", status.getCodeId()) + "','" +
           stringPrintf("%d", status.getCodeLength()) + "','" +
           changeSymbol(status.getSubmitIp()) + "','" +
           stringPrintf("%d", status.getErrorId()) + 
           "')";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection; 
  return ret;
}

int DatabaseInterface::addMail(const Mail& mail){
  Connection* connection = createConnection("localhost",
                                           "root",
                                           "noahoak",
                                           "onlinejudgetest");
  string query;
  query += "insert into mails(topic_id, title, content, unread, time, ";
  query += "to_user, from_user) values('";
  query += stringPrintf("%d", mail.getTopicId())+"','" +
           changeSymbol(mail.getTitle()) + "','" +
           changeSymbol(mail.getContent()) + "','" +
           changeSymbol(mail.getRead()?"N":"Y") + "','" +
           changeSymbol(mail.getTime()) + "','" +
           changeSymbol(mail.getToUser()) + "','" +
           changeSymbol(mail.getFromUser()) + 
           "')";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  query = "update mails set topic_id = mail_id where topic_id = '-1'";
  ret = connection->excuteUpdate(query);
  connection->sclose();
  delete connection; 
  return ret;
}

int DatabaseInterface::addUser(const User& user){
  Connection* connection = createConnection("localhost",
                                           "root",
                                           "noahoak",
                                           "onlinejudgetest");
  string query;
  query += "insert into users(user_id, email, show_email, submits, solveds, ";
  query += "available, last_login_ip, last_login_time, volume, language, ";
  query += "password, reg_time, nickname, school, permission, share_code, ";
  query += "indentify_code) values('"; 
  query += changeSymbol(user.getId()) + "','" +
           changeSymbol(user.getEmail()) + "','" +
           changeSymbol(user.getShowEmail()?"Y":"N") + "','" +
           stringPrintf("%d",users.getSubmit()) + "','" +
           stringPrintf("%d",users.getSolved()) + "','" +
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
  query += changeSymbol(str) + "','";
  query += changeSymbol(user.getShareCode()?"Y":"N") + "','" +
           changeSymbol(user.getIndentifyCode()) + 
           "')";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection; 
  return ret; 
}

int DatabaseInterface::addCode(const Code& code){
	Connection* connection = createConnection("localhost",
                                           "root",
                                           "noahoak",
                                           "onlinejudgetest");
  string query;
  query += "insert into codes(share, code_content) values('";
  query += changeSymbol(code.getShare()?"Y":"N") + "','" +
           changeSymbol(code.getCodeContent()) + 
           "')";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection; 
  return ret;
}

int DatabaseInterface::addError(const Error& error){
	Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "insert into errors(content) values('";
  query += changeSymbol(error.getContent()) + 
           "')";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection; 
  return ret;
}

int DatabaseInterface::addDiscuss(const Discuss& discuss){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "insert into discusses(reply_id, topic_id, user_id, problem_id, ";
  query += "contest_id, title, content, time) values('";
  query += stringPrintf("%d", discuss.getReplyId()) + "','" + 
           stringPrintf("%d", discuss.getTopicId()) + "','" + 
           changeSymbol(discuss.getUserId()) + "','" +
           stringPrintf("%d", discuss.getProblemId()) + "','" +
           stringPrintf("%d", discuss.getContestId()) + "','" +
           changeSymbol(discuss.getTitle()) + "','" +
           changeSymbol(discuss.getContent()) + "','" +
           changeSymbol(discuss.getDate()) + "','" + 
           changeSymbol(discuss.getAvailable()?"Y":"N") + 
           "')";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  query = "update discusses set topic_id = message_id where topic_id = '-1'";
  ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateContest(const Contest& contest){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "update contests set ";
  query += "public_id = '" + stringPrintf("%d",contest.getPublicId()) + "', " +
           "title = '" + changeSymbol(contest.getTitle()) + "', " +
           "description = '" + changeSymbol(contest.getDescription()) + "', " +
           "start_time = '" + changeSymbol(contest.getStartTime()) + "', " +
           "end_time = '" + changeSymbol(contest.getEndTime()) + "', " +
           "contest_type = '" + changeSymbol(contest.getType()) + 
           "' where contest_id = '" + stringPrintf("%d", contest.getContestId()) + 
           "'";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::deleteDiscuss(const Discuss& discuss){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "delete from discusses where message_id = '";
  query += stringPrintf("%d", discuss.getMessageId()) + "'";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::disableDiscuss(const Discuss& discuss){
   Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "update discusses set available = 'N' where topic_id = '";
  query += stringPrintf("%d", discuss.getTopicId()) + "'";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateDiscuss(const Discuss& discuss){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "update discusses set ";
  query += "reply_id = '" + stringPrintf("%d", discuss.getReplyId()) + "', " + 
           "topic_id = '" + stringPrintf("%d", discuss.getTopicId()) + "', " + 
           "user_id = '" + changeSymbol(discuss.getUserId()) + "', " +
           "problem_id = '" + stringPrintf("%d", discuss.getProblemId()) + "', " +
           "contest_id = '" + stringPrintf("%d", discuss.getContestId()) + "', " +
           "title = '" + changeSymbol(discuss.getTitle()) + "', " +
           "content = '" + changeSymbol(discuss.getContent()) + "', " +
           "available = '" + changeSymbol(discuss.getAvailable()?"Y":"N") + "'," +
           "time = '" + changeSymbol(discuss.getDate()) + 
           "' where message_id = '" + stringPrintf("%d", discuss.getMessageId()) + 
           "'";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  query = "update discusses set topic_id = message_id where topic_id = '-1'";
  ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int updateError(const Error& error){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "update errors set ";
  query += "content = '" + changeSymbol(error.getContent()) + 
           "' where error_id = '" + stringPrintf("%d", error.getErrorId()) + 
           "'";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateStatus(const Status& status){
	Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "update statuses set ";
  query += "user_id = '" + changeSymbol(status.getUserId()) + "'," +
           "problem_id = '" + stringPrintf("%d", status.getProblemId()) + "'," +
           "contest_id = '" + stringPrintf("%d", status.getContestId()) + "'," +
           "time = '" + stringPrintf("%d", status.getTime()) + "'," +
           "memory = '" + stringPrintf("%d", status.getMemory()) + "'," +
           "submit_time = '" + changeSymbol(status.getSubmitTime()) + "'," +
           "result = '" + stringPrintf("%d", status.getResult()) + "'," +
           "language= '" + stringPrintf("%d", status.getLanguage()) + "'," +
           "code_id = '" + stringPrintf("%d", status.getCodeId()) + "'," +
           "code_length = '" + stringPrintf("%d", status.getCodeLength()) + "'," +
           "submit_ip = '" + changeSymbol(status.getSubmitIp()) + "'," +
           "error_id = '" + stringPrintf("%d", status.getErrorId()) + 
           "' where status_id = '" + stringPrintf("%d", status.getStatusId()) +
           "'";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateUser(const User& user){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "update users set "; 
  query += "email = '" + changeSymbol(user.getEmail()) + "'," +
           "show_email = '" + changeSymbol(user.getShowEmail()?"Y":"N") + "'," +
           "submits = '" + stringPrintf("%d",user.getSubmit()) + "'," +
           "solveds = '" + stringPrintf("%d",user.getSolved()) + "'," +
           "available = '" + changeSymbol(user.getAvailable()?"Y":"N") + "'," +
           "last_login_ip = '" + changeSymbol(user.getLastLoginIp()) + "'," +
           "last_login_time = '" + changeSymbol(user.getLastLoginTime()) + "'," +
           "volume = '" + stringPrintf("%d",user.getVolume()) + "'," +
           "language = '" + stringPrintf("%d",user.getLanguage()) + "'," +
           "password = '" + changeSymbol(user.getPassword()) + "'," +
           "reg_time = '" + changeSymbol(user.getRegTime()) + "'," +
           "nickname = '" + changeSymbol(user.getNickname()) + "'," +
           "school = '" + changeSymbol(user.getSchool()) + "',";
  string str="----";
  if (user.getPermission() & 0x02 )
    str += "A";
  else
    str += "-";
  if (user.getPermission() & 0x01 )
    str += "V";
  else
    str += "-";
  query += "permission = '" + changeSymbol(str) + "',";
  query += "share_code = '" + changeSymbol(user.getShareCode()?"Y":"N") + "'," +
           "indentify_code = '" + changeSymbol(user.getIndentifyCode()) +
           "' where user_id = '" + changeSymbol(user.getId()) +
           "'";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateNews(const News& news){
	Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "update news set "; 
  query += "publishtime = '" + changeSymbol(news.getPublishTime()) + "'," +
           "title = '" + changeSymbol(news.getTitle()) + "'," +
           "content = '" + changeSymbol(news.getContent()) + 
           "' where news_id = '" + stringPrintf("%d", news.getNewsId()) + 
           "'";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::setCodeSharing(const Code& code){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "update codes set ";
  query += "share = '" + changeSymbol(code.getShare()?"Y":"N") +
           "' where code_id = '" + stringPrintf("%d", code.getCodeId()) + 
           "'";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::deleteDiscuss(const Discuss& discuss){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "delete from discusses where message_id = '";
  query += stringPrintf("%d", discuss.getMessageId()) + "'";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::disableDiscuss(const Discuss& discuss){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "update discusses set available = 'N' where topic_id = '";
  query += stringPrintf("%d", discuss.getTopicId()) + "'";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}


int DatabaseInterface::deleteError(const Error& error){
	Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "delete from errors where error_id = '";
  query += stringPrintf("%d", error.getErrorId()) + "'";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::disableProblem(const Problem& problem){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "update problems set available = 'N' where problem_id = '";
  query += stringPrintf("%d", problem.getProblemId()) + "'";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}
int DatabaseInterface::disableUser(const User& users){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  query += "update users set available = 'N' where user_id = '";
  query += changeSymbol(users.getId()) + "'";
  cout << query << endl;
  cout << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}


int DatabaseInterface::addFilePathtoProblem(const File& file, const Problem& problem){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  cout << "Connection:" << connection->connect() << endl;
  query += "insert into files(path, style) values('";
  query += changeSymbol(file.getPath()) + "','" +
           stringPrintf("%d", file.getType()) + 
           "')";
  cout << query << endl;
  int ret = connection->excuteUpdate(query);
  query = "select file_id from files where path = '" + 
          changeSymbol(file.getPath()) + "' and style = '" +
          stringPrintf("%d", file.getType()) + 
          "'";
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()){
    query = "insert into filestoproblems values('"; 
    query += stringPrintf("%d", result_set.getInt("file_id")) + "','" + 
             stringPrintf("%d", problem.getProblemId()) + 
             "')";
    cout << query << endl;
    ret = connection->excuteUpdate(query);
  }
  result_set.close();
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::addFilePathtoContest(const File& file, const Contest& contest){
	Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query;
  cout << "Connection:" << connection->connect() << endl;
  query += "insert into files(path, style) values('";
  query += changeSymbol(file.getPath()) + "','" +
           stringPrintf("%d", file.getType()) + 
           "')";
  cout << query << endl;
  int ret = connection->excuteUpdate(query);
  query = "select file_id from files where path = '" + 
          changeSymbol(file.getPath()) + "' and style = '" +
          stringPrintf("%d", file.getType()) + 
          "'";
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()){
    query = "insert into filestocontests values('"; 
    query += stringPrintf("%d", result_set.getInt("file_id")) + "','" + 
             stringPrintf("%d", contest.getContestId()) + 
             "')";
    cout << query << endl;
    ret = connection->excuteUpdate(query);
  }
  result_set.close();
  connection->close();
  delete connection;
  return ret;
}

Code DatabaseInterface::getCode(int code_id){
	Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  Code code(code_id);
  string query = "select * from codes where code_id = '" + 
                 stringPrintf("%d", code_id) + "'";
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  if (result_set.next()){
    code.setShare((result_set.getString("share") == string("Y")));
    code.setCodeContent(result_set.getString("code_content"));
    result_set.close();
    connection->close();
    delete connection;
    return code;
  } else {
  	result_set.close();
  	connection->close();
  	delete connection;
    return Code(0);
  }
}

Contest DatabaseInterface::getContest(int contest_id){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  Contest contest(contest_id);
  string query = "select * from contests where contest_id = '" + 
                 stringPrintf("%d", contest_id) + "'";
  connection.connect();
  Result result_set= connection.excuteQuery(query);
  if (result_set.next()){
  	contest.setPublicId(result_set.getInt("public_id"));
  	contest.setTitle(result_set.getString("title"));
  	contest.setStartTime(result_set.getString("start_time"));
  	contest.setEndTime(result_set.getString("end_time"));
  	contest.setType(result_set.getString("contest_type"));
  	contest.setDescription(result_set.getString("description"));
  	result_set.close();
    connection.close();
    delete connection;
    return contest;
  } else {
  	result_set.close();
    connection.close();
    delete connection;
    return Contest(0);
  }
}

Discuss DatabaseInterface::getDiscuss(int discuss_id){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  Discuss discuss(discuss_id);
  string query = "select * from discusses where message_id = '" + 
                 stringPrintf("%d", discuss_id) + "'";
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  if (result_set.next()){
  	discuss.setAvailable(result_set.getString("available") == string("Y") );
  	discuss.setContent(result_set.getString("content"));
  	discuss.setContestId(result_set.getInt("contest_id"));
  	discuss.setDate(result_set.getString("time"));
  	discuss.setProblemId(result_set.getInt("problem_id"));
  	discuss.setReplyId(result_set.getInt("reply_id"));
  	discuss.setTilte(result_set.getString("title"));
  	discuss.setTopicId(result_set.getInt("topic_id"));
  	discuss.setUserId(result_set.getString("user_id"));
  	result_set.close();
    connection->close();
    delete connection;
    return discuss;
  } else {
  	result_set.close();
    connection->close();
    delete connection;
    return Discuss(0);
  }
}

Error DatabaseInterface::getError(int error_id){
	Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  Error error(error_id);
  string query = "select * from errors where error_id = '" + 
                 stringPrintf("%d", error_id) + "'";
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  if (result_set.next()){
  	error.setContent(result_set.getString("content"));
  	result_set.close();
    connection->close();
    delete conection;
    return error;
  } else {
  	result_set.close();
    connection->close();
    delete connection;
    return Error(0);
  }
}

Mail DatabaseInterface::getMail(int mail_id){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  Mail mail(mail_id);
  string query = "select * from mails where mail_id = '" + 
                 stringPrintf("%d", mail_id) + "'";
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  if (result_set.next()){
  	mail.setFromUser(result_set.getString("from_user"));
  	mail.setContent(result_set.getString("content"));
  	mail.setRead(result_set.getString("unread") != "Y");
  	mail.setTime(result_set.getString("time"));
  	mail.setTitle(result_set.getString("title"));
  	mail.setTopicId(result_set.getInt("topic_id"));
  	mail.setToUser(result_set.getString("to_user"));
  	result_set.close();
    connection->close();
    return mail;
  } else {
  	result_set.close();
    connection->close();
    return Mail(0);
  }
}

User DatabaseInterface::getUserInfo(const string& user_id){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  User users(user_id);
  string query = "select * from users where user_id = '" + 
                 changeSymbol(user_id) + "'";
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  if (result_set.next()){
  	users.setPassword(result_set.getString("password"));
  	users.setEmail(result_set.getString("email"));
  	users.setShowEmail(result_set.getString("show_email") == string("Y"));
    users.setNickname(result_set.getString("nickname"));
    users.setSchool(result_set.getString("school"));
    users.setSubmit(result_set.getInt("submits"));
    users.setSolved(result_set.getInt("solveds"));
    users.setShareCode(result_set.getString("share_code") == string("Y"));
    users.setVolume(result_set.getInt("volume"));
    users.setLanguage(result_set.getInt("language"));
    users.setAvailable(result_set.getString("available") == string("Y"));
    users.setLastLoginIp(result_set.getString("last_login_ip"));
    users.setLastLoginTime(result_set.getString("last_login_time"));
    users.setRegTime(result_set.getString("reg_time"));
    users.setPermission(result_set.getInt("permission"));
    users.setIndentifyCode(result_set.getString("indentify_code"));
  	result_set.close();
    connection->close();
    delete connection;
    return users;
  } else {
  	result_set.close();
    connection.->close();
    delete connection;
    return User("NULL");
  }
}

Problem DatabaseInterface::getProblem(int problem_id){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  Problem problem(problem_id);
  string query = "select * from problems where problem_id = '" + 
                 stringPrintf("%d",problem_id) + "'";
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  if (result_set.next()){
  	problem.setAccepted(result_set.getInt("accepted"));
  	problem.setAddinTime(result_set.getString("addin_time"));
  	problem.setAvailable(result_set.getString("available") == string("Y"));
  	problem.setCaseTimeLimit(result_set.getInt("case_time_limit"));
  	problem.setDescription(result_set.getString("description"));
  	problem.setHint(result_set.getString("hint"));
  	problem.setInput(result_set.getString("input"));
  	problem.setMemoryLimit(result_set.getInt("memory_limit"));
  	problem.setOutput(result_set.getString("output"));
  	problem.setSampleInput(result_set.getString("sample_input"));
  	problem.setSampleOutput(result_set.getString("sample_output"));
  	problem.setSolvedUsers(result_set.getInt("solved_users"));
  	problem.setSource(result_set.getString("source"));
  	problem.setSpj(result_set.getString("spj") == string("Y"));
  	problem.setStandardMemoryLimit(result_set.getInt("standard_memory_limit"));
  	problem.setStandardTimeLimit(result_set.getInt("standard_time_limit"));
  	problem.setSubmit(result_set.getInt("submit"));
  	problem.setSubmitUsers(result_set.getInt("submit_users"));
  	problem.setTimeLimit(result_set.getInt("time_limit"));
  	problem.setTitle(result_set.getString("title"));
  	problem.setVersion(result_set.getInt("version"));
  	result_set.close();
    connection->close();
    delete connection;
    return problem;
  } else {
  	result_set.close();
    connection->close();
    delete connetion;
    return Problem(0);
  }
}

ProblemList DatabaseInterface::getProblemList(const ProblemInfo& problem_info){
	ProblemList problem_list;
	bool first = true;
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  ProblemListItem item;
  string query = "select * from problems ";
  if (problem_info.problem_id){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "problem_id = '" + stringPrintf("%d", problem_info.problem_id) + 
             "' ";
  }
  if (problem_info.title != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "title like '%" + changeSymbol(problem_info.title) + "%' ";
  }
  if (problem_info.source != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "source like '%" + changeSymbol(problem_info.source) + "%' ";
  } 
  
  if (problem_info.related_contest){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "problem_id in (select * from problemtocontests where contest_id = '" 
             + stringPrintf("%d", problem_info.related_contest) + "') ";
  }
  
  query += " limit " + stringPrintf("%d, 100", problem_info.page_id*100);
  cout << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.accepted = result_set.getInt("accepted");
  	item.problem_id = result_set.getInt("problem_id");
  	item.submit = result_set.getInt("submit");
  	item.title = result_set.getString("title");
    iter.ac = 0;
  	problem_list.push_back(item);
  }
  result_set.close();
  connection->close();
  delete connection;
  return problem_list;
}

RankList DataInterface::getRankList(const RankListInfo& ranklist_info){
  RankList ranklist;
  RankListItem item;
  bool first = true;
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query = "select * from users order by solveds, submits desc";
  query += " limit " + stringPrintf("%d, 25", ranklist_info.page_id*25);
  cout << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.user_id = result_set.getString("user_id");
  	item.nickname = result_set.getString("nickname");
  	item.solved = result_set.getInt("solveds");
  	item.submit = result_set.getInt("submits");
  	ranklist.push_back(item);
  }
  cout << "ok" << endl;
  result_set.close();
  connection->close();
  delete connection;
  return ranklist;
}

DiscussList DatabaseInterface::getDiscussList(const DiscussInfo& discuss_info){
  DiscussList discuss_list;
	bool first = true;
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  DiscussListItem item;
  string query = "select * from discusses ";
  if (discuss_info.title != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "title like '%" + changeSymbol(discuss_info.title) + "%' ";
  }
  if (discuss_info.problem_id){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "problem_id = '" + stringPrintf("%d", discuss_info.problem_id) + 
             "' ";
  }
  if (discuss_info.contest_id){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "contest_id = '" + stringPrintf("%d", discuss_info.contest_id) + 
             "' ";
  }
  if (discuss_info.user_id != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "user_id like '%" + changeSymbol(discuss_info.user_id) + 
             "%' ";
  }
  query += " limit " + stringPrintf("%d, 25", discuss_info.page_id*25);
  cout << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.contest_id = result_set.getInt("contest_id");
  	item.date = result_set.getString("time");
  	item.discuss_id = result_set.getInt("message_id");
  	item.problem_id = result_set.getInt("problem_id");
  	item.reply_id = result_set.getInt("reply_id");
  	item.title = result_set.getString("title");
  	item.topic_id = result_set.getInt("topic_id");
  	item.user_id = result_set.getString("user_id");
  	discuss_list.push_back(item);
  }
  result_set.close();
  connection->close();
  delete connection;
  return discuss_list;
}

FileList DatabaseInterface::getProblemFile(const Problem& problem){
	FileList filelist;
  File item;
  bool first = true;
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query = "select * from files where file_id in (select file_id from";
         query += " filestoproblems where problem_id = '" + 
                  stringPrintf("%d", problem.getProblemId()) + "')";
  cout << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.setFileId(result_set.getInt("file_id"));
  	item.setPath(result_set.getString("path"));
  	item.setType(result_set.getInt("style"));
  	filelist.push_back(item);
  }
  result_set->close();
  connection->close();
  delete connection;
  return filelist;
}

FileList DatabaseInterface::getContestFile(const Contest& contest){
  FileList filelist;
  File item;
  bool first = true;
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query = "select * from files where file_id in (select file_id from";
         query += " filestocontests where contest_id = '" + 
                  stringPrintf("%d", contest.getContestId()) + "')";
  cout << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.setFileId(result_set.getInt("file_id"));
  	item.setPath(result_set.getString("path"));
  	item.setType(result_set.getInt("style"));
  	filelist.push_back(item);
  }
  result_set.close();
  connection->close();
  delete connection;
  return filelist;
}

ContestList DatabaseInterface::getContestList(const ContestInfo& contest_info){
  ContestList contest_list;
	bool first = true;
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  ContestListItem item;
  string query = "select * from contests ";
  if (contest_info.title != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "title like '%" + changeSymbol(contest_info.title) + "%' ";
  }
  if (contest_info.description != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "description like '%" + changeSymbol(contest_info.description) + "%' ";
  }
  query += " limit " + stringPrintf("%d, 25", contest_info.page_id*25);
  cout << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.title = result_set.getString("title");
  	item.type = result_set.getString("contest_type");
  	item.start_time = result_set.getString("start_time");
  	item.end_time = result_set.getString("end_time");
  	contest_list.push_back(item);
  }
  result_set.close();
  connection->close();
  delete connection;
  return contest_list;
}

MailList getMailList(const MailInfo& mail_info){
  MailList mail_list;
	bool first = true;
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  MailListItem item;
  string query = "select * from mails ";
  if (mail_info.title != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "title like '%" + changeSymbol(mail_info.title) + "%' ";
  }
  if (mail_info.user_id != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    if (mail_info.is_recv)
      query += "to_user = '" + changeSymbol(mail_info.user_id) + "' ";
    else
      query += "from_user = '" + changeSymbol(mail_info.user_id) + "' ";
  }
  query += " limit " + stringPrintf("%d, 25", mail_info.page_id*25);
  cout << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.date = result_set.getString("time");
  	item.from_user = result_set.getString("from_user");
  	item.mail_id = result_set.getInt("mail_id");
  	item.title = result_set.getString("title");
  	item.to_user = result_set.getString("to_user");
  	item.read = (result_set.getString("unread") == "N");
  	mail_list.push_back(item);
  }
  result_set.close();
  connection->close();
  delete connection;
  return mail_list;
}

NewsList DatabaseInterface::getNewsList(const NewsInfo& news_info){
	NewsList newslist;
  NewsListItem item;
  bool first = true;
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query = "select * from news ";
  if (news_info.title != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "title like '%" + changeSymbol(news_info.title) + "%' ";
  }
  query += " order by publishtime desc ";
  query += " limit " + stringPrintf("%d, 25", news_info.page_id*25);
  cout << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.news_id = result_set.getInt("news_id");
  	item.title = result_set.getString("title");
  	item.time = result_set.getString("publishtime");
  	newslist.push_back(item);
  }
  result_set.close();
  connection->close();
  delete connection;
  return newslist;
}

StatusList DatabaseInterface::getNoSearchStatus(){
  StatusList statuslist;
  Status item;
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query = "select * from statuses ";
  query += " order by submit_time desc ";
  query += " limit 0, 25";
  cout << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.setStatusId(result_set.getInt("status_id"));
	  item.setUseId(result_set.getString("user_id"));
	  item.setProblemId(result_set.getInt("problem_id"));
	  item.setContestId(result_set.getInt("contest_id"));
	  item.setTime(result_set.getInt("time"));
	  item.setMemory(result_set.getInt("memory"));
	  item.setSubmitTime(result_set.getString("submit_time"));
	  item.setResult(result_set.getInt("result"));
	  item.SetLanguage(result_set.getInt("language"));
	  item.setCodeId(result_set.getInt("code_id"));
	  item.setCodeLength(result_set.getInt("code_length"));
	  item.setSubmitIp(result_set.getString("submit_ip"));
	  item.setErrorId(result_set.getInt("error_id"));
  	statuslist.push_back(item);
  }
  result_set.close();
  connection->close();
  delete connection;
  return statuslist;
}

StatusList DatabaseInterface::getSearchStatus(const StatusInfo& status_info){
  StatusList statuslist;
  Status item;
  bool first = true;
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query = "select * from statuses ";
  if (status_info.user_id != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "user_id like '%" + changeSymbol(status_info.user_id) + "%' ";
  }
  if (status_info.problem_id){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "problem_id = '" + stringPrintf("%d", status_info.problem_id) + "' ";
  }
  if (status_info.result != -1){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "result = '" + stringPrintf("%d", status_info.result) + "' ";
  }
  if (status_info.contest_id){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "contest_id = '" + stringPrintf("%d", status_info.contest_id) + "' ";
  }
  if (status_info.language != -1){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "language = '" + stringPrintf("%d", status_info.language) + "' ";
  }
  if (status_info.share_code_enable){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "code_id in (select code_id from codes where ";
    query += "share = '";
    query += changeSymbol((status_info.share_code)?"Y":"N") + "') ";
  }
  query += "order by submit_time desc limit " + 
           stringPrintf("%d, 25", status_info.page_id*25);
  cout << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.setStatusId(result_set.getInt("status_id"));
	  item.setUseId(result_set.getString("user_id"));
	  item.setProblemId(result_set.getInt("problem_id"));
	  item.setContestId(result_set.getInt("contest_id"));
	  item.setTime(result_set.getInt("time"));
	  item.setMemory(result_set.getInt("memory"));
	  item.setSubmitTime(result_set.getString("submit_time"));
	  item.setResult(result_set.getInt("result"));
	  item.SetLanguage(result_set.getInt("language"));
	  item.setCodeId(result_set.getInt("code_id"));
	  item.setCodeLength(result_set.getInt("code_length"));
	  item.setSubmitIp(result_set.getString("submit_ip"));
	  item.setErrorId(result_set.getInt("error_id"));
  	statuslist.push_back(item);
  }
  result_set.close();
  connection->close();
  delete conection;
  return statuslist;
}

ContestList DatabaseInterface::getUpcomingContest(){
  Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  ContestList contest_list;
  ContestListItem item;
  string now_time = getLocalTimeAsString("%Y-%m-%d %H:%M:%S");
  string query = "select * from contests where start_time > '";
         query += changeSymbol(now_time) + "' order by start_time";
  query += " limit 0,3";
  cout << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.title = result_set.getString("title");
  	item.type = result_set.getString("contest_type");
  	item.start_time = result_set.getString("start_time");
  	item.end_time = result_set.getString("end_time");
  	item.contest_id = result_set.getInt("contest_id");
  	contest_list.push_back(item);
  }
  result_set.close();
  connection->close();
  return contest_list;
}

User DatabaseInterface::getMostDiligenPlayer(){
	Connection* connection = createConnection("localhost",
                                            "root",
                                            "noahoak",
                                            "onlinejudgetest");
  string query = "select user_id from statuses "
                 "group by user_id having count(*) >= ALL (select count(*) from statuses "
                 "group by user_id ) limit 0,1";
  //cout << query << endl;
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  string user_id = "NULL";
  //cout << user_id << endl;
  if(result_set.next()){
  	user_id = result_set.getString("user_id");
  	cout << user_id << endl;
  }
  result_set.close();
  connection->close();
  delete connection;
  return getUserInfo(user_id);
}

Connection* DatabaseInterface::createConnection(const string& host,
                                               const string& user,
                                               const string& password,
                                               const string& database){
  return new MysqlConnection(host, user, password, database);
}

Connection* DatabaseInterface::createConnection(){
	string host;
	string user;
	string password;
	string database;
  return new MysqlConnection(host, user, password, database);
}


