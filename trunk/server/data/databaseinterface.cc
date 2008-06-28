#include <iostream>
#include <set>
#include <algorithm>

#include "data/databaseinterface.h"
#include "util/filetype.h"

using namespace std;

DatabaseInterface* DatabaseInterface::instance = NULL;

int DatabaseInterface::addContest(const Contest& contest_){
  Connection* connection = createConnection(); 
  LOG(INFO) << "Connection:" << connection->connect();
  Contest contest = contest_;
  if (contest.getType() == "N" || contest.getType() == "Y") {
    string select_query = "select max(public_id) as post_id from contests";
    LOG(INFO) << select_query;
    Result ret = connection->excuteQuery(select_query);
    if (ret.next()) {
      contest.setPublicId(ret.getInt("post_id") + 1);
    } else {
      contest.setPublicId(1);
    }
    ret.close();
  }
  string query;
  query += "insert into contests(public_id, title, description,"; 
  query += "start_time, end_time, contest_type, version, available)"; 
  query += " values('";
  query += stringPrintf("%d",contest.getPublicId()) + "','" +
           changeSymbol(contest.getTitle()) + "','" +
           changeSymbol(contest.getDescription()) + "','" +
           contest.getStartTime() + "','" +
           contest.getEndTime() + "','" +
           contest.getType() + "','" +
           stringPrintf("%d", contest.getVersion()) + "','" +
           changeSymbol(contest.getAvailable()?"Y":"N") + 
           "')";
  LOG(INFO) << query << endl;
  connection->excuteUpdate(query);
  query = "select LAST_INSERT_ID() as contest_id";
  int contest_id = 0;
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    contest_id = result_set.getInt("contest_id");
  }
  LOG(DEBUG) << "Before return contest_id";
  result_set.close();
  connection->close();
  delete connection;
  return contest_id;
}

int DatabaseInterface::disableContest(const Contest& contest) {
  Connection* connection = createConnection();
  string query = "update contests set available = '";
  query += changeSymbol(contest.getAvailable()?"Y":"N") + 
           "' where contest_id = '" + 
           stringPrintf("%d", contest.getContestId()) + "'";
  LOG(INFO) << query;
  LOG(INFO) << "Connection:" << connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::addInputtoOutput(int input_id, int output_id) {
  Connection* connection = createConnection();
  string query = "insert into intooutfiles(in_id, out_id) values ";
  query += "(" + stringPrintf("\'%d\',\'%d\'", input_id, output_id) + ")";
  LOG(INFO) << query;
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::addProblemListtoContest(const Contest& contest,
                            const ProblemIdList& problem_list){
  Connection* connection = createConnection();
  int in_contest_id = 1;
  ProblemIdList::const_iterator problem_iter = problem_list.begin();
  int ret = 0;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  while(problem_iter != problem_list.end()){
    string query;
    query += "insert into problemtocontests(problem_id, contest_id, "; 
    query += "in_contest_id) values('";
    query += stringPrintf("%d", *problem_iter) + "','" +
             stringPrintf("%d", contest.getContestId()) + "','" +
             stringPrintf("%d", in_contest_id) + 
             "')";
    LOG(INFO) << query << endl;
    ret = connection->excuteUpdate(query);
    if (ret)
      LOG(INFO) << "Add Problem " << *problem_iter << "to Contest "
           << contest.getContestId() << endl;
    in_contest_id++;
    problem_iter++;
  }
  connection->close();
  delete connection;
  return ret;                             
}

int DatabaseInterface::addUserListtoContest(const Contest& contest,const UserSet& user_set){
  Connection* connection = createConnection();
  UserSet::const_iterator user_iter = user_set.begin();
  int ret = 0;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  while(user_iter != user_set.end()){
    string query;
    query += "insert into contestpermission(user_id, contest_id) values('";
    query += changeSymbol(*user_iter) + "','" +
             stringPrintf("%d", contest.getContestId()) +
             "')";
    LOG(INFO) << query << endl;
    ret = connection->excuteUpdate(query);
    if (ret)
      LOG(INFO) << "Add User " << *user_iter << "to Contest "
           << contest.getContestId() << endl;
    user_iter++;
  }
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::addNews(const News& news){
  Connection* connection = createConnection();
  string query;
  query += "insert into news(publishtime, title, content) values('"; 
  query += changeSymbol(getLocalTimeAsString("%Y-%m-%d %H:%M:%S")) + "','" +
           changeSymbol(news.getTitle()) + "','" +
           changeSymbol(news.getContent()) + 
           "')";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection; 
  return ret;
}


int DatabaseInterface::addProblemSubmit(int problem_id, int op) {
	Connection* connection = createConnection();
  connection->connect();
  string query = "update problems set submit = submit + '";
  query += stringPrintf("%d", op) + "' where problem_id = '";
  query += stringPrintf("%d", problem_id) + "'"; 
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection; 
  return ret;
}

int DatabaseInterface::addProblemSolved(int problem_id, int op) {
	Connection* connection = createConnection();
  connection->connect();
  string query = "update problems set accepted = accepted + '";
  query += stringPrintf("%d", op) + "' where problem_id = '";
  query += stringPrintf("%d", problem_id) + "'"; 
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection; 
  return ret;
}

int DatabaseInterface::addProblemUserSolved(const Status& status , int op) { 
  Connection* connection = createConnection();
  string query;
  if (op == -1)
    query = "update problems set solved_users = solved_users - 1 where ";  
  else 
    query = "update problems set solved_users = solved_users + 1 where ";
  query += "problem_id = '" + stringPrintf("%d", status.getProblemId()) + "' and ";
  query += " not exist (select * from statuses where status_id != '";
  query += stringPrintf("%d", status.getStatusId()) + "' and user_id ";
  query += " = '" + changeSymbol(status.getUserId()) + "' and ";
  query += " result = '" + stringPrintf("%d", ACCEPTED) + "' and ";
  query += " problem_id = problems.problem_id and type = 'N')";
  LOG(INFO) << query;
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::addProblemUserSubmit(const Status& status , int op) { 
  Connection* connection = createConnection();
  string query;
  if (op == -1)
    query = "update problems set submit_users = submit_users - 1 where ";  
  else 
    query = "update problems set submit_users = submit_users + 1 where ";
  query += "problem_id = '" + stringPrintf("%d", status.getProblemId()) + "' and ";
  query += " not exist (select * from statuses where status_id != '";
  query += stringPrintf("%d", status.getStatusId()) + "' and user_id ";
  query += " = '" + changeSymbol(status.getUserId()) + "' and ";
  query += " problem_id = problems.problem_id and type = 'N')";
  LOG(INFO) << query;
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::addProblem(const Problem& problem){
	Connection* connection = createConnection();
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
           changeSymbol(getLocalTimeAsString("%Y-%m-%d %H:%M:%S")) + "','" +
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
  
  //query += "select LAST_INSERT_ID()";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  connection->excuteUpdate(query);
  query = "select LAST_INSERT_ID() as problem_id";
  int problem_id = 0;
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    problem_id = result_set.getInt("problem_id");
    LOG(DEBUG) << stringPrintf("problem_id:%d", problem_id);
  }
  LOG(INFO) << stringPrintf("problem:%d", problem_id);
  result_set.close();
  connection->close();
  delete connection; 
  return problem_id;
}

int DatabaseInterface::getProblemListNum(const ProblemInfo& problem_info) {
  Connection* connection = createConnection();
  bool first = true;
  bool search = false;
  string query = "select count(*) as num from problems ";
  if (problem_info.problem_id){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    search = true;
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
    search = true;
    query += "title like '%" + changeSymbol(problem_info.title) + "%' ";
  }
  if (problem_info.source != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    search = true;
    query += "source like '%" + changeSymbol(problem_info.source) + "%' ";
  } 
  
  if (problem_info.related_contest){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    search = true;
    query += "problem_id in (select * from problemtocontests where contest_id = '" 
             + stringPrintf("%d", problem_info.related_contest) + "') ";
  }

  connection->connect();
  int ret = 0;
  if (!search) {
    query = "select problem_id as num from problems where problem_id";
    query += " >= all (select problem_id from problems)";
  }
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
      ret = result_set.getInt("num")/100 - 9;
  }
  result_set.close();
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::getContestListNum(bool normal) {
  Connection* connection = createConnection();
  connection->connect();
  string buf;
  if (normal)
    buf = " != 'V' ";
  else
    buf = " == 'V' ";
  int ret = 0;
  string query = "select contest_id from contests where contest_id";
  query += " >= all (select contest_id from contests where contest_type ";
  query += buf + ") and contest_type " + buf;
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    ret = result_set.getInt("contest_id")/100 + 1;
  }
  result_set.close();
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateProblem(const Problem& problem) {
  Connection* connection = createConnection();
  string query;
  query += "update problems set ";
  query += "title = '" + changeSymbol(problem.getTitle()) + "'," +
           "description = '" + changeSymbol(problem.getDescription()) + "'," +
           "input = '" + changeSymbol(problem.getInput()) + "'," +
           "output = '" + changeSymbol(problem.getOutput()) + "'," +
           "sample_input = '" + changeSymbol(problem.getSampleInput()) + "'," +
           "sample_output = '" + changeSymbol(problem.getSampleOutput()) + "'," +
           "hint = '" + changeSymbol(problem.getHint()) + "'," +
           "source = '" + changeSymbol(problem.getSource()) + "'," +
           "time_limit = '" + stringPrintf("%d",problem.getTimeLimit()) + "'," +
           "case_time_limit = '" + stringPrintf("%d",problem.getCaseTimeLimit()) + "'," +
           "memory_limit = '" + stringPrintf("%d",problem.getMemoryLimit()) + "'," +
           "version = version + 1, " + 
           "spj = '" + changeSymbol(problem.getSpj()?"Y":"N") + 
           "' where problem_id = '" + stringPrintf("%d",problem.getProblemId())+ "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection; 
  return ret;
}

int DatabaseInterface::updateProblemStandardLimit(const Problem& problem){
  Connection* connection = createConnection();
  string query;
  query += "update problems set ";
  query += "standard_time_limit = '" + stringPrintf("%d",problem.getTimeLimit()) + "'," +
           "standard_memory_limit = '" + stringPrintf("%d",problem.getMemoryLimit()) + "'," +
           "' where problem_id = '" + stringPrintf("%d",problem.getProblemId())+ "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection; 
  return ret;
}

map<string, string> DatabaseInterface::getProblemInAndOutFile(const Problem& problem) {
  map<string, string> in_and_out_path;
  Connection* connection = createConnection();
  string query;
  query = "select t1.path as in_path, t2.path as out_path from files as t1, files as t2, ";
  query += "intooutfiles where in_id = t1.file_id and out_id = t2.file_id and t1.file_id in";
  query += " (select file_id from filestoproblems where problem_id = ";
  query += stringPrintf("'%d' and version = '%d')", problem.getProblemId(), problem.getVersion());
  LOG(INFO) << query;
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    in_and_out_path[result_set.getString("in_path")] = result_set.getString("out_path");
  }
  result_set.close();
  connection->close();
  delete connection;
  return in_and_out_path;
}

string DatabaseInterface::getProblemSpjFile(const Problem& problem) {
  string path = "";
  Connection* connection = createConnection();
  string query;
  query = "select path from files where style = " + stringPrintf("'%d' and ", SPJ);
  query += "file_id in (select file_id from filestoproblems where problem_id = ";
  query += stringPrintf("'%d' and version = '%d')", problem.getProblemId(), problem.getVersion());
  LOG(INFO) << query;
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    path = result_set.getString("path");
  }
  result_set.close();
  connection->close();
  delete connection;
  return path;
}

int DatabaseInterface::addStatus(const Status& status){
  Connection* connection = createConnection();
  string query;
  query += "insert into statuses(user_id, problem_id, contest_id, time, ";
  query += "memory, submit_time, result, language, code_id, code_length, ";
  query += "submit_ip, error_id, type) values('";
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
           stringPrintf("%d", status.getErrorId()) + "','" +
           changeSymbol(status.getType()) + 
           "')";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  connection->excuteUpdate(query);
  query = "select LAST_INSERT_ID() as status_id";
  int status_id = 0;
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    status_id = result_set.getInt("status_id");
  }
  result_set.close();
  connection->close();
  delete connection; 
  return status_id;
}

string DatabaseInterface::getProblemStandardSource(int problem_id, int& language) {
  string ret = "no";
  Connection* connection = createConnection();
  string query;
  query += "select path from files where style = '" + stringPrintf("%d", STANDARD_SOURCE);
  query += "' and file_id in (select file_id from filestoproblems where problem_id = '";
  query += stringPrintf("%d", problem_id) + "')";
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    ret = result_set.getString("path");
  }
  connection->close();
  delete connection;
  string::size_type pos = ret.find_last_of(".");
  string suffix("NULL");
  language = -1;
  if (pos != string::npos) {
    suffix = ret.substr(pos+1);
  }
  if (suffix == "c") {
    language = GCC;
  } else if (suffix == "cc") {
    language = GPP;
  } else if (suffix == "pas") {
    language = PASCAL;
  } else if (suffix == "java") {
    language = JAVA;
  }
  return ret;
}

int DatabaseInterface::addMail(const Mail& mail){
  Connection* connection = createConnection();
  string query;
  query += "insert into mails(topic_id, title, content, unread, time, ";
  query += "to_user, from_user, reader_del, writer_del) values('";
  query += stringPrintf("%d", mail.getTopicId())+"','" +
           changeSymbol(mail.getTitle()) + "','" +
           changeSymbol(mail.getContent()) + "','" +
           changeSymbol(mail.getRead()?"N":"Y") + "','" +
           changeSymbol(mail.getTime()) + "','" +
           changeSymbol(mail.getToUser()) + "','" +
           changeSymbol(mail.getFromUser()) + "','" +
           changeSymbol(mail.getReaderDel()?"Y":"N") + "','" +
           changeSymbol(mail.getWriterDel()?"Y":"N") +
           "')";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  query = "update mails set topic_id = mail_id where topic_id = '-1'";
  ret = connection->excuteUpdate(query);
  connection->close();
  delete connection; 
  return ret;
}

int DatabaseInterface::addUser(const User& user){
  Connection* connection = createConnection();
  string query;
  query += "insert into users(user_id, email, show_email, submits, solveds, ";
  query += "available, last_login_ip, last_login_time, volume, language, ";
  query += "password, reg_time, nickname, school, permission, share_code, ";
  query += "indentify_code) values('"; 
  query += changeSymbol(user.getId()) + "','" +
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
  query += changeSymbol(str) + "','";
  query += changeSymbol(user.getShareCode()?"Y":"N") + "','" +
           changeSymbol(user.getIndentifyCode()) + 
           "')";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection; 
  return ret; 
}

int DatabaseInterface::addCode(const Code& code){
	Connection* connection = createConnection();
  string query;
  query += "insert into codes(share, code_content) values('";
  query += changeSymbol(code.getShare()?"Y":"N") + "','" +
           changeSymbol(code.getCodeContent()) + 
           "')";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  connection->excuteUpdate(query);
  query = "select LAST_INSERT_ID() as code_id";
  int code_id = 0;
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    code_id = result_set.getInt("code_id");
  }
  result_set.close();
  connection->close();
  delete connection; 
  return code_id;
}

int DatabaseInterface::addError(const Error& error){
	Connection* connection = createConnection();
  string query;
  query += "insert into errors(content) values('";
  query += changeSymbol(error.getContent()) + 
           "')";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  connection->excuteUpdate(query);
  query = "select LAST_INSERT_ID() as error_id";
  int error_id = 0;
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    error_id = result_set.getInt("error_id");
  }
  result_set.close();
  connection->close();
  delete connection; 
  return error_id;
}

int DatabaseInterface::addDiscuss(const Discuss& discuss){
  Connection* connection = createConnection();
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
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  query = "update discusses set topic_id = message_id where topic_id = '-1'";
  ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateContest(const Contest& contest){
  Connection* connection = createConnection();
  string query;
  query += "update contests set ";
  query += "public_id = '" + stringPrintf("%d",contest.getPublicId()) + "', " +
           "title = '" + changeSymbol(contest.getTitle()) + "', " +
           "description = '" + changeSymbol(contest.getDescription()) + "', " +
           "start_time = '" + changeSymbol(contest.getStartTime()) + "', " +
           "end_time = '" + changeSymbol(contest.getEndTime()) + "', " +
           "contest_type = '" + changeSymbol(contest.getType()) +
           "', version = version + 1 ";
  query += " where contest_id = '" + stringPrintf("%d", contest.getContestId()) + 
           "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::deleteDiscuss(const Discuss& discuss){
  Connection* connection = createConnection();
  string query;
  query += "delete from discusses where message_id = '";
  query += stringPrintf("%d", discuss.getMessageId()) + "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::disableDiscuss(const Discuss& discuss){
   Connection* connection = createConnection();
  string query;
  query += "update discusses set available = 'N' where topic_id = '";
  query += stringPrintf("%d", discuss.getTopicId()) + "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateDiscuss(const Discuss& discuss){
  Connection* connection = createConnection();
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
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  query = "update discusses set topic_id = message_id where topic_id = '-1'";
  ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateError(const Error& error){
  Connection* connection = createConnection();
  string query;
  query += "update errors set ";
  query += "content = '" + changeSymbol(error.getContent()) + 
           "' where error_id = '" + stringPrintf("%d", error.getErrorId()) + 
           "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateStatus(const Status& status){
	Connection* connection = createConnection();
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
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateUser(const User& user){
  Connection* connection = createConnection();
  string query;
  query += "update users set "; 
  query += "email = '" + changeSymbol(user.getEmail()) + "'," +
           "show_email = '" + changeSymbol(user.getShowEmail()?"Y":"N") + "'," +
           "available = '" + changeSymbol(user.getAvailable()?"Y":"N") + "'," +
           "last_login_ip = '" + changeSymbol(user.getLastLoginIp()) + "'," +
           "last_login_time = '" + changeSymbol(user.getLastLoginTime()) + "'," +
           "volume = '" + stringPrintf("%d",user.getVolume()) + "'," +
           "language = '" + stringPrintf("%d",user.getLanguage()) + "'," +
           "nickname = '" + changeSymbol(user.getNickname()) + "'," +
           "school = '" + changeSymbol(user.getSchool()) + "',";
  query += "share_code = '" + changeSymbol(user.getShareCode()?"Y":"N") + "'," +
           "indentify_code = '" + changeSymbol(user.getIndentifyCode()) +
           "' where user_id = '" + changeSymbol(user.getId()) +
           "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateUserPermission(const User& user){
  Connection* connection = createConnection();
  string query;
  query += "update users set "; 
  string str = "----";
  if (user.getPermission() & 0x02)
    str += "A";
  else
    str += "-";
  if (user.getPermission() & 0x01)
    str += "V";
  else 
    str += "-";
  query += "permission = '" + changeSymbol(str) + "' " +
           " where user_id = '" + changeSymbol(user.getId()) +
           "'";
  LOG(INFO) << query;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateUserPassword(const User& user) { 
  Connection* connection = createConnection();
  string query;
  query += "update users set "; 
  query += "password = '" + changeSymbol(user.getPassword()) +
           "' where user_id = '" + changeSymbol(user.getId()) +
           "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateNews(const News& news){
	Connection* connection = createConnection();
  string query;
  query += "update news set "; 
  query += "title = '" + changeSymbol(news.getTitle()) + "'," +
           "content = '" + changeSymbol(news.getContent()) + 
           "' where news_id = '" + stringPrintf("%d", news.getNewsId()) + 
           "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::setCodeSharing(const Code& code){
  Connection* connection = createConnection();
  string query;
  query += "update codes set ";
  query += "share = '" + changeSymbol(code.getShare()?"Y":"N") +
           "' where code_id = '" + stringPrintf("%d", code.getCodeId()) + 
           "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}
/*
int DatabaseInterface::deleteDiscuss(const Discuss& discuss){
  Connection* connection = createConnection();
  string query;
  query += "delete from discusses where message_id = '";
  query += stringPrintf("%d", discuss.getMessageId()) + "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::disableDiscuss(const Discuss& discuss){
  Connection* connection = createConnection();
  string query;
  query += "update discusses set available = 'N' where topic_id = '";
  query += stringPrintf("%d", discuss.getTopicId()) + "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}
*/

int DatabaseInterface::deleteError(const Error& error){
	Connection* connection = createConnection();
  string query;
  query += "delete from errors where error_id = '";
  query += stringPrintf("%d", error.getErrorId()) + "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::disableProblem(const Problem& problem){
  Connection* connection = createConnection();
  string query;
  query += "update problems set available = '" + 
           changeSymbol((problem.getAvailable()?"Y":"N")) + 
           "' where problem_id = '";
  query += stringPrintf("%d", problem.getProblemId()) + "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}
int DatabaseInterface::disableUser(const User& users){
  Connection* connection = createConnection();
  string query;
  query += "update users set available = 'N' where user_id = '";
  query += changeSymbol(users.getId()) + "'";
  LOG(INFO) << query << endl;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}


int DatabaseInterface::addFilePathtoProblem(const File& file, const Problem& problem){
  Connection* connection = createConnection();
  string query;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  query += "insert into files(path, style) values('";
  query += changeSymbol(file.getPath()) + "','" +
           stringPrintf("%d", file.getType()) + 
           "')";
  LOG(INFO) << query << endl;
  int ret = connection->excuteUpdate(query);
  int file_id = 0;
  /*query = "select file_id from files where path = '" + 
          changeSymbol(file.getPath()) + "' and style = '" +
          stringPrintf("%d", file.getType()) + "' and disable = 'N" +  
          "'";
  Result result_set = connection->excuteQuery(query);*/
  query = "select LAST_INSERT_ID() as file_id";
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()){
    query = "insert into filestoproblems values('";
    file_id = result_set.getInt("file_id");
    query += stringPrintf("%d", file_id) + "','" + 
             stringPrintf("%d", problem.getProblemId()) + "','" +
             stringPrintf("%d", problem.getVersion()) + 
             "')";
    LOG(INFO) << query << endl;
    ret = connection->excuteUpdate(query);
  }
  result_set.close();
  connection->close();
  delete connection;
  return file_id;
}

int DatabaseInterface::addFilePathtoContest(const File& file, const Contest& contest){
	Connection* connection = createConnection();
  string query;
  LOG(INFO) << "Connection:" << connection->connect() << endl;
  query += "insert into files(path, style) values('";
  query += changeSymbol(file.getPath()) + "','" +
           stringPrintf("%d", file.getType()) + 
           "')";
  LOG(INFO) << query << endl;
  int ret = connection->excuteUpdate(query);
  int file_id = 0;
  /*query = "select file_id from files where path = '" + 
          changeSymbol(file.getPath()) + "' and style = '" +
          stringPrintf("%d", file.getType()) + "' and disable ='N" + 
          "'";
  Result result_set = connection->excuteQuery(query);*/
  query = "select LAST_INSERT_ID() as file_id ";
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()){
    query = "insert into filestocontests values('"; 
    file_id = result_set.getInt("file_id");
    query += stringPrintf("%d", file_id) + "','" + 
             stringPrintf("%d", contest.getContestId()) + "','" +
             stringPrintf("%d", contest.getVersion()) + 
             "')";
    LOG(INFO) << query << endl;
    ret = connection->excuteUpdate(query);
  }
  result_set.close();
  connection->close();
  delete connection;
  return file_id;
}

Code DatabaseInterface::getCode(int code_id){
	Connection* connection = createConnection();
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
  Connection* connection = createConnection();
  Contest contest(contest_id);
  string query = "select * from contests where contest_id = '" + 
                 stringPrintf("%d", contest_id) + "'";
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  if (result_set.next()){
  	contest.setPublicId(result_set.getInt("public_id"));
  	contest.setTitle(result_set.getString("title"));
  	contest.setStartTime(result_set.getString("start_time"));
  	contest.setEndTime(result_set.getString("end_time"));
  	contest.setType(result_set.getString("contest_type"));
  	contest.setDescription(result_set.getString("description"));
    contest.setVersion(result_set.getInt("version"));
    contest.setAvailable(result_set.getString("available") == "Y");
  	result_set.close();
    connection->close();
    delete connection;
    return contest;
  } else {
  	result_set.close();
    connection->close();
    delete connection;
    return Contest(0);
  }
}

Discuss DatabaseInterface::getDiscuss(int discuss_id){
  Connection* connection = createConnection();
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
  	discuss.setTitle(result_set.getString("title"));
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
	Connection* connection = createConnection();
  Error error(error_id);
  string query = "select * from errors where error_id = '" + 
                 stringPrintf("%d", error_id) + "'";
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  if (result_set.next()){
  	error.setContent(result_set.getString("content"));
  	result_set.close();
    connection->close();
    delete connection;
    return error;
  } else {
  	result_set.close();
    connection->close();
    delete connection;
    return Error(0);
  }
}

Mail DatabaseInterface::getMail(int mail_id){
  Connection* connection = createConnection();
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
    mail.setReaderDel(result_set.getString("reader_del") == "Y");
    mail.setWriterDel(result_set.getString("writer_del") == "Y");
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
  Connection* connection = createConnection();
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
    string permission_string = result_set.getString("permission");
    int permission = 0;
    if (permission_string.find_first_of("A") != string::npos) {
       permission |= 0x02;
    }
    if (permission_string.find_first_of("V") != string::npos) {
      permission |= 0x01;
    }
    users.setPermission(permission);
    users.setIndentifyCode(result_set.getString("indentify_code"));
  	result_set.close();
    connection->close();
    delete connection;
    return users;
  } else {
  	result_set.close();
    connection->close();
    delete connection;
    return User("NULL");
  }
}

DiscussList DatabaseInterface::getReplyDiscussList(int message_id) {
  Connection* connection = createConnection();
  string query = "select * from discusses where reply_id = '" +
          stringPrintf("%d", message_id) + "' and available = 'Y'";
  DiscussList discuss_list;
  DiscussListItem item;
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
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

TopicSet DatabaseInterface::getDiscussTopicSet(const DiscussInfo& discuss_info) {
  Connection* connection = createConnection();
  TopicSet topic_set;
	bool first = true;
  string query = "select topic_id, time from discusses ";
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
  if (first) {
    query += "where ";
    first = false;
  } else {
    query += "and ";
  }
  query += " available = 'Y'";
  query += " order by time desc ";
  query += " limit " + stringPrintf("%d, 10", discuss_info.page_id*10);
  LOG(INFO) << query << endl;
  set<int> topic;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
    int topic_id = result_set.getInt("topic_id");
    if (topic.count(topic_id) == 0){
  	  topic_set.push_back(topic_id);
      topic.insert(topic_id);
    }
  }
  result_set.close();
  connection->close();
  delete connection;
  return topic_set;
}

Problem DatabaseInterface::getProblem(int problem_id){
  Connection* connection = createConnection();
  Problem problem(problem_id);
  string query = "select * from problems where problem_id = '" + 
                 stringPrintf("%d",problem_id) + "'";
  LOG(INFO) <<  query;
  connection->connect();
  Result result_set = connection->excuteQuery(query);
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
    delete connection;
    return Problem(0);
  }
}

ProblemList DatabaseInterface::getProblemList(const ProblemInfo& problem_info){
	ProblemList problem_list;
	bool first = true;
  bool nosearch = true;
  Connection* connection = createConnection();
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
    nosearch = false;
  }
  if (problem_info.title != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "title like '%" + changeSymbol(problem_info.title) + "%' ";
    nosearch = false;
  }
  if (problem_info.source != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "source like '%" + changeSymbol(problem_info.source) + "%' ";
    nosearch = false;
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
    nosearch = false;
  }
  if (nosearch) {
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += stringPrintf(" problem_id <= %d and problem_id > %d", 
                          (problem_info.page_id + 1) * 100 + 1000, 
                          problem_info.page_id * 100 + 1000);
  }
  else
    query += " limit " + stringPrintf("%d, 100", problem_info.page_id*100);
  LOG(INFO) << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.accepted = result_set.getInt("accepted");
  	item.problem_id = result_set.getInt("problem_id");
  	item.submit = result_set.getInt("submit");
  	item.title = result_set.getString("title");
    item.source = result_set.getString("source");
    item.ac = 0;
    item.available = (result_set.getString("available") == "Y");
  	problem_list.push_back(item);
  }
  result_set.close();
  connection->close();
  delete connection;
  LOG(DEBUG) << "here is ok";
  return problem_list;
}

ContestProblemList DatabaseInterface::getContestProblemList(int contest_id) {
  ContestProblemList problem_list;
  Contest contest = getContest(contest_id);
  Connection* connection = createConnection();
  map<int, int> problem_accepted;
  map<int, int> problem_submit;
  ContestProblemItem item;
  string query = "select t1.problem_id, in_contest_id, title from problems as t1, ";
  query += "problemtocontests as t2 where t1.problem_id = t2.problem_id and ";
  query += "contest_id = '" + stringPrintf("%d", contest_id) + "' ";
  query += "order by in_contest_id ";
  query += " limit 0, 100";
  LOG(INFO) << query << endl;
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()){
  	item.problem_id = result_set.getInt("problem_id");
    item.in_contest_id = result_set.getInt("in_contest_id");
  	item.title = result_set.getString("title");
    item.total = 0;
    item.accepted = 0;
  	problem_list.push_back(item);
  }
  result_set.close();
  query = "select * from statuses where contest_id = '" + stringPrintf("%d'", contest_id);
  query += "and type = 'N'";
  query += " and submit_time >= '" + contest.getStartTime();
  query += "' and submit_time <= '" + contest.getEndTime();
  query += "' order by submit_time";
  result_set =connection->excuteQuery(query);
  while (result_set.next()) {
    problem_submit[result_set.getInt("problem_id")]++;
    if (result_set.getInt("result") == ACCEPTED) 
      problem_accepted[result_set.getInt("problem_id")]++;
  }
  result_set.close();
  ContestProblemList::iterator iter = problem_list.begin();
  while (iter != problem_list.end()) {
    if (problem_accepted.count(iter->problem_id) > 0)
      iter->accepted = problem_accepted[iter->problem_id];
    if (problem_submit.count(iter->problem_id) > 0) 
      iter->total = problem_submit[iter->problem_id];
    iter++;
  }
  connection->close();
  delete connection;
  return problem_list;
}

ProblemIdList DatabaseInterface::getContestProblems(int contest_id) {
  ProblemIdList problem_list;
  string query = "select problem_id from problemtocontests where contest_id = '";
  query += stringPrintf("%d", contest_id) = "' order by in_contest_id";
  LOG(DEBUG) << query;
  Connection* connection = createConnection();
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    problem_list.push_back(result_set.getInt("problem_id"));
  }
  result_set.close();
  connection->close();
  delete connection;
  return problem_list;
}

int DatabaseInterface::getContestProblemNum(int contest_id){
  Connection* connection = createConnection();
  connection->connect();
  string query = "select count(*) as num from problemtocontests where contest_id = '";
  query += stringPrintf("%d", contest_id) + "'";
  Result result_set = connection->excuteQuery(query);
  int ret = 0;
  if (result_set.next()) {
    ret = result_set.getInt("num");
  }
  result_set.close();
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::disableMail(const string& user_id, int mail_id) {
  Mail mail = getMail(mail_id);
  string query = "update mails set ";
  if (mail.getToUser() == user_id) 
    query += " writer_del = 'Y' ";
  else if(mail.getFromUser() == user_id)
    query += " reader_del = 'Y' ";
  else 
    return -1;
  query += " where mail_id = '" + stringPrintf("%d", mail_id) + "'";
  Connection* connection = createConnection();
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

RankList DatabaseInterface::getRankList(const RankListInfo& ranklist_info){
  RankList ranklist;
  RankListItem item;
  Connection* connection = createConnection();
  string query = "select * from users ";
  switch (ranklist_info.seq) {
    case 0:
      query += "order by solveds desc, submits";
      break;
    case 1:
      query += "order by submits desc, solveds desc";
      break;
    case 2:
      query += "order by solveds/submits desc, solveds desc";
      break;
  }
  query += " limit " + stringPrintf("%d, 25", ranklist_info.page_id*25);
  LOG(INFO) << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.user_id = result_set.getString("user_id");
  	item.nickname = result_set.getString("nickname");
  	item.solved = result_set.getInt("solveds");
  	item.submit = result_set.getInt("submits");
  	ranklist.push_back(item);
  }
  LOG(INFO) << "ok" << endl;
  result_set.close();
  connection->close();
  delete connection;
  return ranklist;
}

DiscussList DatabaseInterface::getDiscussList(const DiscussInfo& discuss_info){
  DiscussList discuss_list;
	bool first = true;
  Connection* connection = createConnection();
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
  query += " order by time desc ";
  query += " limit " + stringPrintf("%d, 20", discuss_info.page_id*20);
  LOG(INFO) << query << endl;
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
  Connection* connection = createConnection();
  string query = "select * from files where file_id in (select file_id from";
  query += " filestoproblems as t1, problems as t2 where t1.problem_id ";
  query += " = t2.problem_id and t1.version = t2.version and ";
  query += " t1.problem_id = '" +
           stringPrintf("%d", problem.getProblemId()) + "') and type = '";
  query += stringPrintf("%d", IMG) + "'";
  LOG(INFO) << query << endl;
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

FileList DatabaseInterface::getContestFile(const Contest& contest){
  FileList filelist;
  File item;
  Connection* connection = createConnection();
  string query = "select * from files where file_id in (select file_id from";
  query += " filestocontests as t1, contests as t2 where t1.contest_id ";
  query += " = t2.contest_id and t1.version = t2.version and ";
  query += " t1.contest_id = '" +  
           stringPrintf("%d", contest.getContestId()) + "') and type = '";
  query += stringPrintf("%d",IMG) + "'";
  LOG(INFO) << query << endl;
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
  Connection* connection = createConnection();
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
   if (contest_info.type != '-'){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    if (contest_info.type == 'C')
      query += "(contest_type = 'N' or contest_type = 'P') ";
    else
      query += "contest_type = '" + stringPrintf("%c",contest_info.type) + "' ";
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
  query += " order by start_time desc ";
  query += " limit " + stringPrintf("%d, 25", contest_info.page_id*25);
  LOG(INFO) << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
    item.contest_id = result_set.getInt("contest_id");
  	item.title = result_set.getString("title");
  	item.type = result_set.getString("contest_type");
  	item.start_time = result_set.getString("start_time");
  	item.end_time = result_set.getString("end_time");
    item.public_id = result_set.getInt("public_id");
    item.available = (result_set.getString("available")  == "Y");
  	contest_list.push_back(item);
  }
  result_set.close();
  connection->close();
  delete connection;
  return contest_list;
}

MailList DatabaseInterface::getMailList(const MailInfo& mail_info){
  MailList mail_list;
	bool first = true;
  Connection* connection = createConnection();
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
    query += " (to_user = '" + changeSymbol(mail_info.user_id) + "' ";
    query += " and reader_del = 'N') ";
    query += " or (from_user = '" + changeSymbol(mail_info.user_id) + "' ";
    query += " and writer_del = 'N') ";
  }
  query += " order by time desc limit " + 
           stringPrintf("%d, 25", mail_info.page_id*25);
  LOG(INFO) << query << endl;
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

int DatabaseInterface::getNewMailCount(const string& user_id){
  Connection* connection = createConnection();
  MailListItem item;
  string query = "select count(*) as new_mail from mails ";
  query += "where unread = 'Y' and reader_del = 'N' and to_user = '" + changeSymbol(user_id) + "'";
  connection->connect();
  int ret = 0;
  Result result_set= connection->excuteQuery(query);
  if (result_set.next()){
    ret = result_set.getInt("new_mail");
  }
  result_set.close();
  connection->close();
  delete connection;
  return ret;
}

NewsList DatabaseInterface::getNewsList(const NewsInfo& news_info){
	NewsList newslist;
  NewsListItem item;
  bool first = true;
  Connection* connection = createConnection();
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
  query += " limit " + stringPrintf("%d, 10", news_info.page_id*10);
  LOG(INFO) << query << endl;
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
  Connection* connection = createConnection();
  string query = "select * from statuses";
  query += " where type = 'N' and ";
  string time = getLocalTimeAsString("%Y-%m-%d %H:%M:%S");
  query += " not exists (select * from contests where statuses.contest_id = contests.contest_id and start_time <= '";
  query += changeSymbol(time) + "' and end_time >= '" + changeSymbol(time) + "') ";
  query += " order by submit_time desc ";
  query += " limit 0, 25";
  LOG(INFO) << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.setStatusId(result_set.getInt("status_id"));
	  item.setUserId(result_set.getString("user_id"));
	  item.setProblemId(result_set.getInt("problem_id"));
	  item.setContestId(result_set.getInt("contest_id"));
	  item.setTime(result_set.getInt("time"));
	  item.setMemory(result_set.getInt("memory"));
	  item.setSubmitTime(result_set.getString("submit_time"));
	  item.setResult(result_set.getInt("result"));
	  item.setLanguage(result_set.getInt("language"));
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

bool DatabaseInterface::canReadCode(const string& user_id, int code_id) {
  bool ret = false;
  Connection* connection = createConnection();
  connection->connect();
  string query = "select code_id from codes where code_id = '" + stringPrintf("%d", code_id);
  query += "' and ( exists (select * from statuses where user_id = '"; 
  query += changeSymbol(user_id) + "' and statuses.code_id = codes.code_id)";
  query += " or ( share = 'Y' and exists (select * from statuses where user_id = '";
  query += changeSymbol(user_id) + "' and result = '" + stringPrintf("%d", ACCEPTED) + 
           "' and problem_id in (select problem_id from statuses where code_id = '" +
           stringPrintf("%d", code_id) + "'))))";
  Result result_set = connection->excuteQuery(query);
  ret = result_set.next();
  result_set.close();
  connection->close();
  delete connection;
  return ret;
}

Status DatabaseInterface::getStatus(int status_id){
  Status item;
  Connection* connection = createConnection();
  string query = "select * from statuses ";
  query += " where status_id = '" + stringPrintf("%d'", status_id);
  LOG(INFO) << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  if(result_set.next()){
  	item.setStatusId(result_set.getInt("status_id"));
	  item.setUserId(result_set.getString("user_id"));
	  item.setProblemId(result_set.getInt("problem_id"));
	  item.setContestId(result_set.getInt("contest_id"));
	  item.setTime(result_set.getInt("time"));
	  item.setMemory(result_set.getInt("memory"));
	  item.setSubmitTime(result_set.getString("submit_time"));
	  item.setResult(result_set.getInt("result"));
	  item.setLanguage(result_set.getInt("language"));
	  item.setCodeId(result_set.getInt("code_id"));
	  item.setCodeLength(result_set.getInt("code_length"));
	  item.setSubmitIp(result_set.getString("submit_ip"));
	  item.setErrorId(result_set.getInt("error_id"));
    item.setType(result_set.getString("type"));
  }
  result_set.close();
  connection->close();
  delete connection;
  return item;
}

StatusList DatabaseInterface::getClientStatusList(const string& user_id) {
  StatusList status_list;
  Status item;
  string query = "select * from statuses where type = 'N' and user_id = '";
  query += changeSymbol(user_id) + "' order by submit_time desc limit 0, 200";
  Connection* connection =  createConnection();
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    item.setStatusId(result_set.getInt("status_id"));
	  item.setUserId(result_set.getString("user_id"));
	  item.setProblemId(result_set.getInt("problem_id"));
	  item.setContestId(result_set.getInt("contest_id"));
	  item.setTime(result_set.getInt("time"));
	  item.setMemory(result_set.getInt("memory"));
	  item.setSubmitTime(result_set.getString("submit_time"));
	  item.setResult(result_set.getInt("result"));
	  item.setLanguage(result_set.getInt("language"));
	  item.setCodeId(result_set.getInt("code_id"));
	  item.setCodeLength(result_set.getInt("code_length"));
	  item.setSubmitIp(result_set.getString("submit_ip"));
	  item.setErrorId(result_set.getInt("error_id"));
  	status_list.push_back(item);
  }
  result_set.close();
  connection->close();
  delete connection;
  return status_list;
}

ProblemIdList DatabaseInterface::getClientProblemList() {
  ProblemIdList problem_list;
  string query = "select problem_id from problems where available = 'Y' order by problem_id";
  Connection* connection = createConnection();
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    problem_list.push_back(result_set.getInt("problem_id"));
  }
  result_set.close();
  connection->close();
  delete connection;
  return problem_list;
}

ContestInfoList DatabaseInterface::getClientContestList() {
  ContestInfoList contest_list;
  ContestInfoItem item;
  Connection* connection = createConnection();
  connection->connect();
  string now_time = getLocalTimeAsString("%Y-%m-%d %H:%M:%S");
  string query = "select contest_id,title from contests where available = 'Y' ";
  query += " and contest_type != 'C' and start_time <= '" + changeSymbol(now_time) +
           "' and end_time >= '" + changeSymbol(now_time) + "' ";
  query += " order by contest_id ";
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    item.contest_id = result_set.getInt("contest_id");
    item.title = result_set.getString("title");
    contest_list.push_back(item);
  }
  result_set.close();
  ContestInfoList::iterator iter = contest_list.begin();
  while (iter != contest_list.end()) {
    query = "select problem_id from problemtocontests where contest_id = '" +
            stringPrintf("%d' ", iter->contest_id) + " order by in_contest_id";
    LOG(DEBUG) << query;
    result_set = connection->excuteQuery(query);
    while (result_set.next()) {
      iter->problem_list.push_back(result_set.getInt("problem_id"));
    }
    result_set.close();
    iter++;
  }
  connection->close();
  delete connection;
  return contest_list;
}

StatusList DatabaseInterface::getSearchStatus(const StatusInfo& status_info){
  StatusList statuslist;
  Status item;
  bool first = true;
  Connection* connection = createConnection();
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
  }else if (status_info.type != "A"){        
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }  
    string time = getLocalTimeAsString("%Y-%m-%d %H:%M:%S");
    query += " not exists (select * from contests where statuses.contest_id = contests.contest_id and start_time <= '";
    query += changeSymbol(time) + "' and end_time >= '" + changeSymbol(time) + "') ";
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
  if (status_info.type != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    if (status_info.type == "A")
      query += "type = 'N' ";
    else
      query += "type = '" + changeSymbol(status_info.type) + "' ";
  }
  query += "order by submit_time desc limit " + 
           stringPrintf("%d, 25", status_info.page_id*25);
  LOG(INFO) << query << endl;
  connection->connect();
  Result result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.setStatusId(result_set.getInt("status_id"));
	  item.setUserId(result_set.getString("user_id"));
	  item.setProblemId(result_set.getInt("problem_id"));
	  item.setContestId(result_set.getInt("contest_id"));
	  item.setTime(result_set.getInt("time"));
	  item.setMemory(result_set.getInt("memory"));
	  item.setSubmitTime(result_set.getString("submit_time"));
	  item.setResult(result_set.getInt("result"));
	  item.setLanguage(result_set.getInt("language"));
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

ContestList DatabaseInterface::getUpcomingContest(){
  Connection* connection = createConnection();
  ContestList contest_list;
  ContestListItem item;
  string now_time = getLocalTimeAsString("%Y-%m-%d %H:%M:%S");
  string query = "select * from contests where start_time > '";
         query += changeSymbol(now_time) + "' and contest_type != 'V' order by start_time";
  query += " limit 0,3";
  LOG(INFO) << query << endl;
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

UserList DatabaseInterface::getMostDiligenPlayer(){
  UserList user_list;
  UserListItem item;
	Connection* connection = createConnection();
  string query = "select * from most_diligen_player";
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  string user_id = "NULL";
  if(result_set.next()){
  	item.user_id = result_set.getString("day_user");
  	LOG(INFO) << user_id << endl;
    user_list.push_back(item);
  	item.user_id = result_set.getString("week_user");
  	LOG(INFO) << user_id << endl;
    user_list.push_back(item);
  	item.user_id = result_set.getString("month_user");
  	LOG(INFO) << user_id << endl;
    user_list.push_back(item);
  }
  result_set.close();
  connection->close();
  delete connection;
  return user_list;
}

int DatabaseInterface::setMailRead(const Mail& mail) {
  Connection* connection = createConnection();
  connection->connect();
  string query = "update mails ";
  query += "set unread = '" + changeSymbol((mail.getRead()?"N":"Y")) + 
           "' where mail_id = '"+ stringPrintf("%d'", mail.getMailId());
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

//ContestRankList DatabaseInterface::getContestRankList(const ContestRankListInfo& contest_ranklist_info){
ContestRankList DatabaseInterface::getContestRankList(int contest_id){
  ContestRankList contest_ranklist;
  ContestRankListItem item;
  ContestProblemTime time_item;
  map<string, ContestRankListItem> ranklist_buf;
  map<int, int> problem_id_to_contest;
  //Contest contest = getContest(contest_ranklist_info.contest_id);
  Contest contest = getContest(contest_id);
  Connection* connection = createConnection();
  connection->connect();
  string query = "select * from problemtocontests where contest_id = '";
  //query += stringPrintf("%d", contest_ranklist_info.contest_id) + "' ";
  query += stringPrintf("%d", contest_id) + "' ";
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()){
  	 int problem_id = result_set.getInt("problem_id");
     problem_id_to_contest[problem_id] = result_set.getInt("in_contest_id");
  }
  result_set.close();
  query = "select * from statuses where contest_id = '";
  //query += stringPrintf("%d", contest_ranklist_info.contest_id) + "' ";
  query += stringPrintf("%d", contest_id);
  query += "' and type = 'N' and submit_time >= '" + contest.getStartTime();
  query += "' and submit_time <= '" + contest.getEndTime() + "' order by submit_time ";
  result_set = connection->excuteQuery(query);
  while (result_set.next()){   
    string user_id = result_set.getString("user_id");
    int problem_id = result_set.getInt("problem_id");
    string submit_time = result_set.getString("submit_time");
    int time = caltime(submit_time, contest.getStartTime());
    int result = result_set.getInt("result");
    if (ranklist_buf.count(user_id) == 0){
      item.accepted = 0;
      item.penalty = 0;
      item.user_id = user_id;
      ranklist_buf[user_id] = item;
    }
    if (ranklist_buf[user_id].problem_penalty.count(problem_id) == 0){
      time_item.ac = false;
      time_item.penalty = 0 ;
      time_item.problem_id = problem_id;
      time_item.submit = 0;
      time_item.in_contest_id = problem_id_to_contest[problem_id];
      ranklist_buf[user_id].problem_penalty[problem_id]=time_item;
    }
    if (!ranklist_buf[user_id].problem_penalty[problem_id].ac)
      ranklist_buf[user_id].problem_penalty[problem_id].submit++;
    if (result == ACCEPTED && !ranklist_buf[user_id].problem_penalty[problem_id].ac){
      ranklist_buf[user_id].accepted++;
      ranklist_buf[user_id].problem_penalty[problem_id].ac = true;
      ranklist_buf[user_id].problem_penalty[problem_id].penalty = time + 
        20*60*(ranklist_buf[user_id].problem_penalty[problem_id].submit - 1);
      ranklist_buf[user_id].penalty += ranklist_buf[user_id].problem_penalty[problem_id].penalty;
    }
  }
  result_set.close();
  connection->close();
  delete connection;
  map<string, ContestRankListItem>::iterator iter = ranklist_buf.begin();
  while (iter != ranklist_buf.end()){
  	iter->second.nickname = getUserInfo(iter->second.user_id).getNickname();
    contest_ranklist.push_back(iter->second);
    iter++;
  }
  sort(contest_ranklist.begin(), contest_ranklist.end());
  return contest_ranklist;
}

bool DatabaseInterface::checkContestAcBefore(const ContestAcBefore& contest_acbefore) {
  bool ret = false;
  Connection* connection = createConnection();
  string query = "select * from statuses ";
  query += " where user_id = '" + changeSymbol(contest_acbefore.user_id);
  query += "' and time < '" + changeSymbol(contest_acbefore.time);
  query += "' and contest_id = '" + stringPrintf("%d", contest_acbefore.contest_id);
  query += "' and problem_id = '" + stringPrintf("%d", contest_acbefore.problem_id);
  query += "' and result = '" + stringPrintf("%d", ACCEPTED) + "' and type = 'N'";
  LOG(INFO) << query;
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  ret = result_set.next();
  result_set.close();
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::updateUserSolved(const Status& status, int op) {
  Connection* connection = createConnection();
  string query;
  connection->connect();
  query = "select * from statuses where status_id != '";
  query += stringPrintf("%d' and ", status.getStatusId()); 
  query += stringPrintf("problem_id = '%d' and ", status.getProblemId()); 
  query += stringPrintf("result = '%d' and ", ACCEPTED);
  query += "user_id = '" + changeSymbol(status.getUserId()) + "' and type = 'N'";
  Result result_set = connection->excuteQuery(query);
  int canAdd = !result_set.next();
  result_set.close();
  int ret = 0;
  if (canAdd) {
    if (op == -1)
      query = "update users set solveds = solveds - 1 where ";  
    else 
      query = "update users set solveds = solveds + 1 where ";
    query += "user_id = '" + changeSymbol(status.getUserId()) + "'";
    LOG(INFO) << query;
    ret = connection->excuteUpdate(query);
  }
  connection->close();
  delete connection;
  return ret;
}


int DatabaseInterface::updateUserSubmit(const User& user, int op) {
  Connection* connection = createConnection();
  string query;
  if (op == -1)
    query = "update users set submits = submits - 1 where ";  
  else 
    query = "update users set submits = submits + 1 where ";
  query += "user_id = '" + changeSymbol(user.getId()) + "'";
  LOG(INFO) << query;
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::getInContestId(int contest_id, int problem_id) {
  bool in_contest_id = -1;
  Connection* connection = createConnection();
  connection->connect();
  string query = "select in_contest_id from problemtocontests ";
  query += "where contest_id = '" + stringPrintf("%d", contest_id) +"' ";
  query += "and problem_id = '" + stringPrintf("%d", problem_id) + "'";
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    in_contest_id = result_set.getInt("in_contest_id");
  }
  result_set.close();
  connection->close();
  delete connection;
  return in_contest_id;
}

ContestStatistics DatabaseInterface::getContestStatistics(int contestId){
  ContestStatistics contest_statistics;
  ContestStatisticsItem item;
  map<int, ContestStatisticsItem> statistics_buf;
  Contest contest = getContest(contestId);
  Connection* connection = createConnection();
  connection->connect();
  string query = "select problem_id, in_contest_id from problemtocontests ";
  query += "where contest_id = '" + stringPrintf("%d", contestId) + "'";
  Result result_set= connection->excuteQuery(query);
  while (result_set.next()){
  	memset(&item, 0, sizeof(item));
    item.problem_id = result_set.getInt("problem_id");
    item.incontest_id = result_set.getInt("in_contest_id");
    statistics_buf[item.problem_id] = item;
  }
  result_set.close();
  UserACSet set;
  query = "select * from statuses where contest_id =' " + 
          stringPrintf("%d", contestId) + "' and type = 'N'";
  query += " and submit_time >= '" + contest.getStartTime();
  query += "' and submit_time <= '" + contest.getEndTime();
  query += "' order by submit_time";
  result_set= connection->excuteQuery(query);
  ProblemUserAC problem_user_ac;
  while (result_set.next()){
  	int problem_id = result_set.getInt("problem_id");
  	int result = result_set.getInt("result");
  	int language = result_set.getInt("language");
    string user_id = result_set.getString("user_id");
    problem_user_ac.user_id = user_id;
    problem_user_ac.problem_id = problem_id;
    if (set.count(problem_user_ac) > 0) {
      continue;
    }
  	statistics_buf[problem_id].Total++;
  	switch (language){
  	  case 0:
  	  case 1:
  	    statistics_buf[problem_id].C_CPP++;
  	    break;
  	  case 2:
  	    statistics_buf[problem_id].Java++;
  	    break;
  	  case 3:
  	    statistics_buf[problem_id].Pascal++;
  	}
  	switch (result){
  		case ACCEPTED:
        problem_user_ac.user_id = user_id;
        problem_user_ac.problem_id = problem_id;
      //  if (set.count(problem_user_ac) == 0) {
  		  statistics_buf[problem_id].AC++;
        set.insert(problem_user_ac);
      //  }
  	    break;
  		case PRESENTATION_ERROR:
  		  statistics_buf[problem_id].PE++;
  	    break;
  		case TIME_LIMIT_EXCEEDED:
  		  statistics_buf[problem_id].TLE++;
  	    break;
  		case MEMORY_LIMIT_EXCEEDED:
  		  statistics_buf[problem_id].MLE++;
  	    break;
  		case WRONG_ANSWER:
  		  statistics_buf[problem_id].WA++;
  	    break;
  		case OUTPUT_LIMIT_EXCEEDED:
  		  statistics_buf[problem_id].OLE++;
  	    break;
  		case COMPILE_ERROR:
  		  statistics_buf[problem_id].CE++;
  	    break;
  		case RUNTIME_ERROR_SIGSEGV:
  		case RUNTIME_ERROR_SIGFPE:
  		case RUNTIME_ERROR_SIGBUS:
  		case RUNTIME_ERROR_SIGABRT:
  		case RUNTIME_ERROR_JAVA:
  		  statistics_buf[problem_id].RE++;
  	    break;
  	}
  }
  result_set.close();
  map<int, ContestStatisticsItem>::iterator iter = statistics_buf.begin();
  while (iter != statistics_buf.end()){
    contest_statistics.push_back(iter->second);
    iter++;
  }
  connection->close();
  delete connection;
  return contest_statistics;
}

ProblemStatistics DatabaseInterface::getProblemStatistics(int problem_id) {
  Connection* connection = createConnection();
  connection->connect();
  ProblemStatistics problem_statistics;
  memset(&problem_statistics, 0, sizeof(problem_statistics));
  problem_statistics.problem_id = problem_id;
  string query = "select * from statuses where problem_id = '" +
                 stringPrintf("%d", problem_id) + "' and type = 'N'";
  LOG(INFO) << query;
  set<string> user_ac_set,
              user_solved_set;
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    int result = result_set.getInt("result");
    int language = result_set.getInt("language");
    string user_id = result_set.getString("user_id");
    problem_statistics.Total++;
    if (user_solved_set.count(user_id) == 0) {
      problem_statistics.User_Total++;
      user_solved_set.insert(user_id);
    }
    switch (language){
  	  case 0:
  	  case 1:
  	    problem_statistics.C_CPP++;
  	    break;
  	  case 2:
  	    problem_statistics.Java++;
  	    break;
  	  case 3:
  	    problem_statistics.Pascal++;
  	}
  	switch (result){
  		case ACCEPTED:
  		  problem_statistics.AC++;
        if (user_ac_set.count(user_id) == 0) {
          problem_statistics.User_AC++;
          user_ac_set.insert(user_id);
        }
  	    break;
  		case PRESENTATION_ERROR:
  		  problem_statistics.PE++;
  	    break;
  		case TIME_LIMIT_EXCEEDED:
  		  problem_statistics.TLE++;
  	    break;
  		case MEMORY_LIMIT_EXCEEDED:
  		  problem_statistics.MLE++;
  	    break;
  		case WRONG_ANSWER:
  		  problem_statistics.WA++;
  	    break;
  		case OUTPUT_LIMIT_EXCEEDED:
  		  problem_statistics.OLE++;
  	    break;
  		case COMPILE_ERROR:
  		  problem_statistics.CE++;
  	    break;
  		case RUNTIME_ERROR_SIGSEGV:
  		case RUNTIME_ERROR_SIGFPE:
  		case RUNTIME_ERROR_SIGBUS:
  		case RUNTIME_ERROR_SIGABRT:
  		case RUNTIME_ERROR_JAVA:
  		  problem_statistics.RE++;
  	    break;
    }
  }
  result_set.close();
  connection->close();
  delete connection;
  return problem_statistics;
}

int DatabaseInterface::disableContestProblems(const Contest& contest) {
  Connection* connection = createConnection();
  connection->connect();
  string query = "delete from problemtocontests where contest_id = '";
  query += stringPrintf("%d", contest.getContestId()) + "'";
  int ret = connection->excuteUpdate(query); 
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::disableContestUsers(const Contest& contest) {
  Connection* connection = createConnection();
  connection->connect();
  string query = "delete from contestpermission where contest_id = '";
  query += stringPrintf("%d", contest.getContestId()) + "'";
  int ret = connection->excuteUpdate(query); 
  connection->close();
  delete connection;
  return ret;
}

bool DatabaseInterface::checkPermission(int contest_id, const string& user_id) {
  Contest contest = getContest(contest_id);
  if (contest.getContestId() == 0) {
    return false;
  } else if (contest.getType() != "P") {
    return true;
  } 
  Connection* connection = createConnection();
  connection->connect();
  string query = "select * from contestpermission where contest_id = '";
  query += stringPrintf("%d' and user_id = '%s'", user_id.c_str());
  Result result_set = connection->excuteQuery(query);
  bool ret = result_set.next();
  result_set.close();
  connection->close();
  delete connection;
  return ret;
}

ProblemSet DatabaseInterface::getUserACProblem(const string& user_id, bool ac) {
  Connection* connection = createConnection();
  ProblemSet problem_set;
  connection->connect();
  string query = "select * from statuses where user_id = '";
  query += changeSymbol(user_id) + "' and result ";
  if (ac) 
    query += " = '" + stringPrintf("%d", ACCEPTED) + "'";
  else
    query += " != '" + stringPrintf("%d", ACCEPTED) + "'";
  query += "and type = 'N'";
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    problem_set.insert(result_set.getInt("problem_id"));
  }
  result_set.close();
  connection->close();
  delete connection;
  return problem_set;
}

StatusList DatabaseInterface::getProblemStatus(const StatusInfo& status_info) {
  StatusList statuslist;
  Status item;
  Connection* connection = createConnection();
  string query = "select distinct(user_id) from statuses as t1 where not exists (select * "
                 "from statuses as t2 where result = '";
  query += stringPrintf("%d' ", ACCEPTED) + "and type = 'N' and problem_id = '" + 
           stringPrintf("%d' ", status_info.problem_id) + " and (t2.time < t1.time "
           "or (t2.time = t1.time and t2.memory < t1.memory)) and t1.user_id = t2.user_id ) "
           "and type  = 'N' and result = '" + stringPrintf("%d' ", ACCEPTED) + 
           "and problem_id = '" + stringPrintf("%d' ", status_info.problem_id) + 
           "order by time, memory, submit_time desc limit " +
           stringPrintf("%d, 25", status_info.page_id * 25);
  LOG(INFO) << query << endl;
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  vector<string> user_id_list;
  while(result_set.next()){
    user_id_list.push_back(result_set.getString("user_id"));
  }
  result_set.close();
  vector<string>::iterator iter = user_id_list.begin();
  while (iter != user_id_list.end()) {
    query = "";
    query += "select * from statuses where type = 'N' and user_id = '" + changeSymbol(*iter) +
    "' and result = '" + stringPrintf("%d", ACCEPTED) + "' and problem_id = '" + 
    stringPrintf("%d",status_info.problem_id) + 
    "' order by time, memory, submit_time desc";
    result_set = connection->excuteQuery(query);
    if (result_set.next()) {
      item.setStatusId(result_set.getInt("status_id"));
      item.setUserId(result_set.getString("user_id"));
      item.setProblemId(result_set.getInt("problem_id"));
      item.setContestId(result_set.getInt("contest_id"));
      item.setTime(result_set.getInt("time"));
      item.setMemory(result_set.getInt("memory"));
      item.setSubmitTime(result_set.getString("submit_time"));
      item.setResult(result_set.getInt("result"));
      item.setLanguage(result_set.getInt("language"));
      item.setCodeId(result_set.getInt("code_id"));
      item.setCodeLength(result_set.getInt("code_length"));
      item.setSubmitIp(result_set.getString("submit_ip"));
      item.setErrorId(result_set.getInt("error_id"));
      statuslist.push_back(item);
    }
    result_set.close();
    iter++;
  }
  connection->close();
  delete connection;
  return statuslist;
}

int DatabaseInterface::updateFileVersion(int problem_id, int contest_id) {
  Connection* connection = createConnection();
  string query;
  connection->connect();
  if (problem_id != -1) {
    query = "update filestoproblems set version = version + 1 where problem_id = '";
    query += stringPrintf("%d", problem_id) + "' and file_id in (select file_id from ";
    query += " files where style in (" + stringPrintf("'%d','%d','%d','%d'))", INFILE, OUTFILE, SPJ, IMG);
  } else {
    query = "update filestocontests set version = version + 1 where contest_id = '";
    query += stringPrintf("%d", contest_id) + "' and file_id in (select file_id from ";
    query += " files where style in (" + stringPrintf("'%d','%d','%d','%d'))", INFILE, OUTFILE, SPJ, IMG);
  }
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int DatabaseInterface::getUserRank(const string& user_id) {
  Connection* connection = createConnection();
  connection->connect();
  int accepted = 0;
  int submit = 0;
  string query = "select * from users where user_id = '";
  query += changeSymbol(user_id) + "'";
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    accepted = result_set.getInt("solveds");
    submit = result_set.getInt("submits");
  }
  result_set.close();
  query = "select count(*) as rank from users where solveds > '";
  query += stringPrintf("%d' or (solveds = '%d' and ", accepted, accepted);
  query += stringPrintf(" submits < '%d')", submit);
  LOG(DEBUG) << query;
  result_set = connection->excuteQuery(query);
  int ret = -1;
  if (result_set.next()) {
    ret = result_set.getInt("rank") + 1;
  }
  result_set.close();
  connection->close();
  delete connection;
  return ret;
}

UserList DatabaseInterface::getUserList(const UserInfo& user_info) {
  Connection* connection = createConnection();
  UserList user_list;
  UserListItem item;
  bool first = true;
  string query = "select * from users ";
  if (user_info.user_id != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "user_id like '%" + changeSymbol(user_info.user_id) + "%' ";
  }
  if (user_info.nickname != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "nickname like '%" + changeSymbol(user_info.nickname) + "%' ";
  }
  query += " order by reg_time";
  query += " limit " + stringPrintf("%d, 25", user_info.page_id*25);
  LOG(INFO) << query;
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    item.user_id = result_set.getString("user_id");
    item.nickname = result_set.getString("nickname");
    item.last_login_time = result_set.getString("last_login_time");
    item.last_login_ip = result_set.getString("last_login_ip");
    item.reg_time = result_set.getString("reg_time");
    user_list.push_back(item);
  }
  connection->close();
  delete connection;
  return user_list;
}

Connection* DatabaseInterface::createConnection(const string& host,
                                               const string& user,
                                               const string& password,
                                               const string& database){
  return new MysqlConnection(host, user, password, database);
}

Connection* DatabaseInterface::createConnection(){
	string host = Configure::getInstance().getDatabaseHost();
	string user = Configure::getInstance().getDatabaseUser();
	string password = Configure::getInstance().getDatabasePassword();
	string database = Configure::getInstance().getDatabaseName();
  return new MysqlConnection(host, user, password, database);
}


