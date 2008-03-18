#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
using namespace std;

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

string host, user, password, database;

string changeSymbol(const string& str){
  string strbuf(str);
  string::size_type pos = 0;
  while(( pos = strbuf.find_first_of("_'\\\"%",pos)) != string::npos){
  	strbuf.insert(pos, 1, '\\');
  	pos += 2;
  	cout << "pos:" << pos << endl;
  	cout << "strbuf: " << strbuf <<endl;
  } 
  return strbuf;
}

int addContest(const Contest& contest){
  MysqlConnection connection(host, user, password, database);
  string query;
  query += "insert into contests(public_id, title, description,"; 
  query +=" start_time, end_time, contest_type) values('";
  query += stringPrintf("%d",contest.getPublicId()) + "','" +
           changeSymbol(contest.getTitle()) + "','" +
           changeSymbol(contest.getDescription()) + "','" +
           changeSymbol(contest.getStartTime()) + "','" +
           changeSymbol(contest.getEndTime()) + "','" +
           changeSymbol(contest.getType()) + 
           "')";
  cout << query << endl;
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}
// to be discuss
int addProblemListtoContest(const Contest& contest,
                            const ProblemList& problem_list){
  MysqlConnection connection(host, user, password, database);
  int in_contest_id = 1;
  ProblemList::const_iterator problem_iter = problem_list.begin();
  int ret;
  cout << "Connection:" << connection.connect() << endl;
  while(problem_iter != problem_list.end()){
    string query;
    query += "insert into problemtocontests(problem_id, contest_id, "; 
    query += "in_contest_id) values('";
    query += stringPrintf("%d", problem_iter->problem_id) + "','" +
             stringPrintf("%d", contest.getContestId()) + "','" +
             stringPrintf("%d", in_contest_id) + 
             "')";
    cout << query << endl;
    ret = connection.excuteUpdate(query);
    if (ret)
      cout << "Add Problem " << problem_iter->problem_id << "to Contest "
           << contest.getContestId() << endl;
    in_contest_id++;
    problem_iter++;
  }
  connection.close();
  return ret;                             
}

int addUserListtoContest(const Contest& contest,const UserList& user_list){
  MysqlConnection connection(host, user, password, database);
  UserList::const_iterator user_iter = user_list.begin();
  int ret;
  cout << "Connection:" << connection.connect() << endl;
  while(user_iter != user_list.end()){
    string query;
    query += "insert into contestpermission(user_id, contest_id) values('";
    query += changeSymbol(user_iter->user_id) + "','" +
             stringPrintf("%d", contest.getContestId()) +
             "')";
    cout << query << endl;
    ret = connection.excuteUpdate(query);
    if (ret)
      cout << "Add User " << user_iter->user_id << "to Contest "
           << contest.getContestId() << endl;
    user_iter++;
  }
  connection.close();
  return ret;
}
int addNews(const News& news){
  MysqlConnection connection(host, user, password, database);
  string query;
  query += "insert into news(publishtime, title, content) values('"; 
  query += changeSymbol(news.getPublishTime()) + "','" +
           changeSymbol(news.getTitle()) + "','" +
           changeSymbol(news.getContent()) + 
           "')";
  cout << query << endl;
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}
/*
int addNotice(const Notice& notice);
*/
int addProblem(const Problem& problem){
	MysqlConnection connection(host, user, password, database);
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
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int addStatus(const Status& status){
  MysqlConnection connection(host, user, password, database);
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
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int addMail(const Mail& mail){
  MysqlConnection connection(host, user, password, database);
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
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  query = "update mails set topic_id = mail_id where topic_id = '-1'";
  ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}
//to be continue;
int addUser(const User& users){
  MysqlConnection connection(host, user, password, database);
  string query;
  query += "insert into users(user_id, email, show_email, submits, solveds, ";
  query += "available, last_login_ip, last_login_time, volume, language, ";
  query += "password, reg_time, nickname, school, permission, share_code, ";
  query += "indentify_code) values('"; 
  query += changeSymbol(users.getId()) + "','" +
           changeSymbol(users.getEmail()) + "','" +
           changeSymbol(users.getShowEmail()?"Y":"N") + "','" +
           stringPrintf("%d",users.getSubmit()) + "','" +
           stringPrintf("%d",users.getSolved()) + "','" +
           changeSymbol(users.getAvailable()?"Y":"N") + "','" +
           changeSymbol(users.getLastLoginIp()) + "','" +
           changeSymbol(users.getLastLoginTime()) + "','" +
           stringPrintf("%d",users.getVolume()) + "','" +
           stringPrintf("%d",users.getLanguage()) + "','" +
           changeSymbol(users.getPassword()) + "','" +
           changeSymbol(users.getRegTime()) + "','" +
           changeSymbol(users.getNickname()) + "','" +
           changeSymbol(users.getSchool()) + "','";
  string str="----";
  if (users.getPermission() & 0x02 )
    str += "A";
  else
    str += "-";
  if (users.getPermission() & 0x01 )
    str += "V";
  else
    str += "-";
  query += changeSymbol(str) + "','";
  query += changeSymbol(users.getShareCode()?"Y":"N") + "','" +
           changeSymbol(users.getIndentifyCode()) + 
           "')";
  cout << query << endl;
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int addCode(const Code& code){
	MysqlConnection connection(host, user, password, database);
  string query;
  query += "insert into codes(share, code_content) values('";
  query += changeSymbol(code.getShare()?"Y":"N") + "','" +
           changeSymbol(code.getCodeContent()) + 
           "')";
  cout << query << endl;
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int addError(const Error& error){
	MysqlConnection connection(host, user, password, database);
  string query;
  query += "insert into errors(content) values('";
  query += changeSymbol(error.getContent()) + 
           "')";
  cout << query << endl;
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int addDiscuss(const Discuss& discuss){
  MysqlConnection connection(host, user, password, database);
  string query;
  query += "insert into discusses(reply_id, topic_id, user_id, problem_id, ";
  query += "contest_id, title, content, time, available) values('";
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
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  query = "update discusses set topic_id = message_id where topic_id = '-1'";
  ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int updateContest(const Contest& contest){
  MysqlConnection connection(host, user, password, database);
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
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}
/*
int updateProblemListtoContest(const Contest& constest,const ProblemList& problem_list);
int updateUserListtoContest(const Contest& contest,const UserList& user_list);
*/
int updateDiscuss(const Discuss& discuss){
  MysqlConnection connection(host, user, password, database);
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
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  query = "update discusses set topic_id = message_id where topic_id = '-1'";
  ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int updateStatus(const Status& status){
	MysqlConnection connection(host, user, password, database);
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
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int updateUser(const User& users){
  MysqlConnection connection(host, user, password, database);
  string query;
  query += "update users set "; 
  query += "email = '" + changeSymbol(users.getEmail()) + "'," +
           "show_email = '" + changeSymbol(users.getShowEmail()?"Y":"N") + "'," +
           "submits = '" + stringPrintf("%d",users.getSubmit()) + "'," +
           "solveds = '" + stringPrintf("%d",users.getSolved()) + "'," +
           "available = '" + changeSymbol(users.getAvailable()?"Y":"N") + "'," +
           "last_login_ip = '" + changeSymbol(users.getLastLoginIp()) + "'," +
           "last_login_time = '" + changeSymbol(users.getLastLoginTime()) + "'," +
           "volume = '" + stringPrintf("%d",users.getVolume()) + "'," +
           "language = '" + stringPrintf("%d",users.getLanguage()) + "'," +
           "password = '" + changeSymbol(users.getPassword()) + "'," +
           "reg_time = '" + changeSymbol(users.getRegTime()) + "'," +
           "nickname = '" + changeSymbol(users.getNickname()) + "'," +
           "school = '" + changeSymbol(users.getSchool()) + "',";
  string str="----";
  if (users.getPermission() & 0x02 )
    str += "A";
  else
    str += "-";
  if (users.getPermission() & 0x01 )
    str += "V";
  else
    str += "-";
  query += "permission = '" + changeSymbol(str) + "',";
  query += "share_code = '" + changeSymbol(users.getShareCode()?"Y":"N") + "'," +
           "indentify_code = '" + changeSymbol(users.getIndentifyCode()) +
           "' where user_id = '" + changeSymbol(users.getId()) +
           "'";
  cout << query << endl;
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int updateNews(const News& news){
	MysqlConnection connection(host, user, password, database);
  string query;
  query += "update news set "; 
  query += "publishtime = '" + changeSymbol(news.getPublishTime()) + "'," +
           "title = '" + changeSymbol(news.getTitle()) + "'," +
           "content = '" + changeSymbol(news.getContent()) + 
           "' where news_id = '" + stringPrintf("%d", news.getNewsId()) + 
           "'";
  cout << query << endl;
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int updateError(const Error& error){
  MysqlConnection connection(host, user, password, database);
  string query;
  query += "update errors set ";
  query += "content = '" + changeSymbol(error.getContent()) + 
           "' where error_id = '" + stringPrintf("%d", error.getErrorId()) + 
           "'";
  cout << query << endl;
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int setCodeSharing(const Code& code){
  MysqlConnection connection(host, user, password, database);
  string query;
  query += "update codes set ";
  query += "share = '" + changeSymbol(code.getShare()?"Y":"N") +
           "' where code_id = '" + stringPrintf("%d", code.getCodeId()) + 
           "'";
  cout << query << endl;
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int deleteDiscuss(const Discuss& discuss){
  MysqlConnection connection(host, user, password, database);
  string query;
  query += "delete from discusses where message_id = '";
  query += stringPrintf("%d", discuss.getMessageId()) + "'";
  cout << query << endl;
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int disableDiscuss(const Discuss& discuss){
  MysqlConnection connection(host, user, password, database);
  string query;
  query += "update discusses set available = 'N' where topic_id = '";
  query += stringPrintf("%d", discuss.getTopicId()) + "'";
  cout << query << endl;
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}


int deleteError(const Error& error){
	MysqlConnection connection(host, user, password, database);
  string query;
  query += "delete from errors where error_id = '";
  query += stringPrintf("%d", error.getErrorId()) + "'";
  cout << query << endl;
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int disableProblem(const Problem& problem){
  MysqlConnection connection(host, user, password, database);
  string query;
  query += "update problems set available = 'N' where problem_id = '";
  query += stringPrintf("%d", problem.getProblemId()) + "'";
  cout << query << endl;
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}
int disableUser(const User& users){
  MysqlConnection connection(host, user, password, database);
  string query;
  query += "update users set available = 'N' where user_id = '";
  query += changeSymbol(users.getId()) + "'";
  cout << query << endl;
  cout << "Connection:" << connection.connect() << endl;
  int ret = connection.excuteUpdate(query);
  connection.close();
  return ret;
}

int addFilePathtoProblem(const File& file, const Problem& problem){
  MysqlConnection connection(host, user, password, database);
  string query;
  cout << "Connection:" << connection.connect() << endl;
  query += "insert into files(path, style) values('";
  query += changeSymbol(file.getPath()) + "','" +
           stringPrintf("%d", file.getType()) + 
           "')";
  cout << query << endl;
  int ret = connection.excuteUpdate(query);
  query = "select file_id from files where path = '" + 
          changeSymbol(file.getPath()) + "' and style = '" +
          stringPrintf("%d", file.getType()) + 
          "'";
  Result result_set = connection.excuteQuery(query);
  if (result_set.next()){
    query = "insert into filestoproblems values('"; 
    query += stringPrintf("%d", result_set.getInt("file_id")) + "','" + 
             stringPrintf("%d", problem.getProblemId()) + 
             "')";
    cout << query << endl;
    ret = connection.excuteUpdate(query);
  }
  result_set.close();
  connection.close();
  return ret;
}

int addFilePathtoContest(const File& file, const Contest& contest){
	MysqlConnection connection(host, user, password, database);
  string query;
  cout << "Connection:" << connection.connect() << endl;
  query += "insert into files(path, style) values('";
  query += changeSymbol(file.getPath()) + "','" +
           stringPrintf("%d", file.getType()) + 
           "')";
  cout << query << endl;
  int ret = connection.excuteUpdate(query);
  query = "select file_id from files where path = '" + 
          changeSymbol(file.getPath()) + "' and style = '" +
          stringPrintf("%d", file.getType()) + 
          "'";
  Result result_set = connection.excuteQuery(query);
  if (result_set.next()){
    query = "insert into filestocontests values('"; 
    query += stringPrintf("%d", result_set.getInt("file_id")) + "','" + 
             stringPrintf("%d", contest.getContestId()) + 
             "')";
    cout << query << endl;
    ret = connection.excuteUpdate(query);
  }
  result_set.close();
  connection.close();
  return ret;
}

/*
FileList getProblemFile(const Problem& problem);
FileList getContestFile(const Contest& contest);
*/
Code getCode(int code_id){
	MysqlConnection connection(host, user, password, database);
  Code code(code_id);
  string query = "select * from codes where code_id = '" + 
                 stringPrintf("%d", code_id) + "'";
  connection.connect();
  Result result_set= connection.excuteQuery(query);
  if (result_set.next()){
    code.setShare((result_set.getString("share") == string("Y")));
    code.setCodeContent(result_set.getString("code_content"));
    result_set.close();
    connection.close();
    return code;
  } else {
  	result_set.close();
    connection.close();
    return Code(0);
  }
}

Contest getContest(int contest_id){
  MysqlConnection connection(host, user, password, database);
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
    return contest;
  } else {
  	result_set.close();
    connection.close();
    return Contest(0);
  }
}
/*
ContestList getContestList(const ContestInfo& contest_info);
ContestRankList getContestRankList(const ContestRankListInfo& contest_ranklist_info);
ContestStatistics getContestStatistics(int contestId);
Discuss getDiscuss(int discuss_id);
DiscussList getDiscussList(const DiscussInfo& discuss_info);
Error getError(int error_id);
Mail getMail(int mail_id);
MailList getMailList(const MailInfo& mail_info);
User getMostDiligenPlayer();
NewsList getNewsList(const NewsInfo& news_info);
StatusList getNoSearchStatus();
StatusList getSearchStatus(const StatusInfo& status_info);
Notice getNotice();
Problem getProblem(int problem_id);
ProblemList getProblemList(const ProblemInfo& problem_info);
RankList getRankList(const RankListInfo& ranklist_info);
Contest getUpcomingContest();
User getUserInfo(int user_id);
UserList getUserList(const UserInfo& user_info);
*/

int main(){
  host = "localhost";
  user = "root";
  password = "noahoak";
  database = "onlinejudgetest";
  News news(1, "2008-03-13 10:00:00", "Apai say.", "Let's fuck.");
  Problem problem(1,
                  "abcdefg",
                  "Adaf",
                  "XXX",
                  "OOO",
                  "XXXX",
                  "OOOO",
                  "",
                  "",
                  "2008-03-13 10:00:00",
                  12,
                  2,
                  64,
                  true,
                  0,
                  0,
                  0,
                  0,
                  12,
                  4,
                  1,
                  false);
  Mail mail(1, -1, "aPai", "ooeyusea", "fuck", "The fuck", true, 
            "2008-03-13 10:00:00");
  Status status(1, "apai", 1, 0, 4, 10, "2008-03-13 10:00:00", 0, 0, 0, 0,
                "192.168.5.123", 0);
  User user("macheng", "654321", "a@a.com", true, "machengabcdefg", "whu", 0, 0, 
            false, 1, 0, true, "", "0000-00-00 00:00:00", "2008-03-14 10:00:00", 
            0, "");
  Code code(1, true, "printf(\"%d\",a.getchar())");
  Error error(1, "Error on macheng.");
  Contest contest(1, 1, "Apai's Apple", "Apai want to eat a lot of apples.",
                  "2008-03-14 10:00:00", "2008-03-04 9:00:00", "N");
  Discuss discuss(6, 0, 5, "Apai", 1, 3, "Apai's", 
                  "Today we will to chi shaokao.", "2004-03-14 10::00:00", true);
  ProblemListItem a;
  File file(0, "/usr/abc/a/in", 01);
  ProblemList problem_list;
  a.problem_id = 1;
  problem_list.push_back(a);
  a.problem_id = 2;
  problem_list.push_back(a);
  a.problem_id = 4;
  problem_list.push_back(a);
  UserList user_list;
  user_list.push_back(user.getUserListItem());
  //user.setId("LiuQian");
  user_list.push_back(user.getUserListItem());
  //user.setId("Apai");
  user_list.push_back(user.getUserListItem());
  //Code code1 = getCode(1);
  Contest contest1 = getContest(1);
  if (contest1.getContestId())
    cout << contest1.getTitle() << " " << contest1.getDescription() << endl;
  cout << addProblem(problem) <<endl;
  return 0;
}

