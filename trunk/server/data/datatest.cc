// do not test the ContestR/S function , getUserList has bug
#include <string>
#include <iostream>
#include <sstream>
#include <iterator>
#include <vector>
#include <map>
using namespace std;
#include <time.h>

#include "info.h"
#include "base/util.h"
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

time_t changeTime(const string& time){
  struct tm time_buf;
  time_buf.tm_year = atoi(time.substr(0,4).c_str()) - 1900;
  time_buf.tm_mon = atoi(time.substr(5,2).c_str()) - 1;
  time_buf.tm_mday = atoi(time.substr(8,2).c_str());
  time_buf.tm_hour = atoi(time.substr(11,2).c_str());
  time_buf.tm_min = atoi(time.substr(14,2).c_str());
  time_buf.tm_sec = atoi(time.substr(17,2).c_str());
  return mktime(&time_buf);
}

int caltime(const string& end_time, const string& start_time){
  double diff = difftime(changeTime(end_time), changeTime(start_time));
  return static_cast<int> (diff);
}

User getUserInfo(const string& user_id);

FileList getProblemFile(const Problem& problem){
	FileList filelist;
  File item;
  bool first = true;
  MysqlConnection connection(host, user, password, database);
  string query = "select * from files where file_id in (select file_id from";
         query += " filestoproblems where problem_id = '" + 
                  stringPrintf("%d", problem.getProblemId()) + "')";
  cout << query << endl;
  connection.connect();
  Result result_set= connection.excuteQuery(query);
  while(result_set.next()){
  	item.setFileId(result_set.getInt("file_id"));
  	item.setPath(result_set.getString("path"));
  	item.setType(result_set.getInt("style"));
  	filelist.push_back(item);
  }
  result_set.close();
  connection.close();
  return filelist;
}

FileList getContestFile(const Contest& contest){
  FileList filelist;
  File item;
  bool first = true;
  MysqlConnection connection(host, user, password, database);
  string query = "select * from files where file_id in (select file_id from";
         query += " filestocontests where contest_id = '" + 
                  stringPrintf("%d", contest.getContestId()) + "')";
  cout << query << endl;
  connection.connect();
  Result result_set= connection.excuteQuery(query);
  while(result_set.next()){
  	item.setFileId(result_set.getInt("file_id"));
  	item.setPath(result_set.getString("path"));
  	item.setType(result_set.getInt("style"));
  	filelist.push_back(item);
  }
  result_set.close();
  connection.close();
  return filelist;
}

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

ContestList getContestList(const ContestInfo& contest_info){
  ContestList contest_list;
	bool first = true;
  MysqlConnection connection(host, user, password, database);
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
  connection.connect();
  Result result_set= connection.excuteQuery(query);
  while(result_set.next()){
  	item.title = result_set.getString("title");
  	item.type = result_set.getString("contest_type");
  	item.start_time = result_set.getString("start_time");
  	item.end_time = result_set.getString("end_time");
  	item.contest_id = result_set.getInt("contest_id");
  	contest_list.push_back(item);
  }
  result_set.close();
  connection.close();
  return contest_list;
}

ContestRankList getContestRankList(const ContestRankListInfo& contest_ranklist_info){
  ContestRankList contest_ranklist;
  ContestRankListItem item;
  ContestProblemTime time_item;
  map<string, ContestRankListItem> ranklist_buf;
  map<int, int> problem_id_to_contest;
  Contest contest = getContest(contest_ranklist_info.contest_id);
  MysqlConnection connection(host, user, password, database);
  connection.connect();
  string query = "select * from problemtocontests where contest_id = '";
  query += stringPrintf("%d", contest_ranklist_info.contest_id) + "' ";
  Result result_set = connection.excuteQuery(query);
  while (result_set.next()){
  	 int problem_id = result_set.getInt("problem_id");
     problem_id_to_contest[problem_id] = result_set.getInt("in_contest_id");
  }
  result_set.close();
  query = "select * from statuses where contest_id = '";
  query += stringPrintf("%d", contest_ranklist_info.contest_id) + "' ";
  query += "order by submit_time ";
  query += " limit " + stringPrintf("%d, 25", contest_ranklist_info.page_id*25);
  result_set = connection.excuteQuery(query);
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
      ranklist_buf[user_id].problem_penalty[problem_id]=time_item;
    }
    if (!ranklist_buf[user_id].problem_penalty[problem_id].ac)
      ranklist_buf[user_id].problem_penalty[problem_id].submit++;
    if (result == 0 && !ranklist_buf[user_id].problem_penalty[problem_id].ac){
      ranklist_buf[user_id].accepted++;
      ranklist_buf[user_id].problem_penalty[problem_id].ac = true;
      ranklist_buf[user_id].problem_penalty[problem_id].penalty = time + 
        20*60*(ranklist_buf[user_id].problem_penalty[problem_id].submit - 1);
      ranklist_buf[user_id].penalty += ranklist_buf[user_id].problem_penalty[problem_id].penalty;
      ranklist_buf[user_id].problem_penalty[problem_id].in_contest_id = 
        problem_id_to_contest[problem_id];
    }
  }
  result_set.close();
  connection.close();
  map<string, ContestRankListItem>::iterator iter = ranklist_buf.begin();
  while (iter != ranklist_buf.end()){
  	iter->second.nickname = getUserInfo(iter->second.user_id).getNickname();
    contest_ranklist.push_back(iter->second);
    iter++;
  }
  return contest_ranklist;
}

ContestStatistics getContestStatistics(int contestId){
  ContestStatistics contest_statistics;
  ContestStatisticsItem item;
  map<int, ContestStatisticsItem> statistics_buf;
  MysqlConnection connection(host, user, password, database);
  connection.connect();
  string query = "select problem_id, in_contest_id from problemtocontests ";
  query += "where contest_id = '" + stringPrintf("%d", contestId) + "'";
  Result result_set= connection.excuteQuery(query);
  while (result_set.next()){
  	memset(&item, 0, sizeof(item));
    item.problem_id = result_set.getInt("problem_id");
    item.problem_id = result_set.getInt("in_contest_id");
    statistics_buf[item.problem_id] = item;
  }
  result_set.close();
  query = "select * from statuses where contest_id =' " + 
          stringPrintf("%d", contestId) + "'";
  result_set= connection.excuteQuery(query);
  while (result_set.next()){
  	int problem_id = result_set.getInt("problem_id");
  	int result = result_set.getInt("result");
  	int language = result_set.getInt("language");
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
  		case 0:
  		  statistics_buf[problem_id].AC++;
  	    break;
  		case 1:
  		  statistics_buf[problem_id].PE++;
  	    break;
  		case 2:
  		  statistics_buf[problem_id].TLE++;
  	    break;
  		case 3:
  		  statistics_buf[problem_id].MLE++;
  	    break;
  		case 4:
  		  statistics_buf[problem_id].WA++;
  	    break;
  		case 5:
  		  statistics_buf[problem_id].OLE++;
  	    break;
  		case 6:
  		  statistics_buf[problem_id].CE++;
  	    break;
  		case 7:
  		case 8:
  		case 9:
  		case 10:
  		case 11:
  		case 12:
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
  connection.close();
  return contest_statistics;
}

Discuss getDiscuss(int discuss_id){
  MysqlConnection connection(host, user, password, database);
  Discuss discuss(discuss_id);
  string query = "select * from discusses where message_id = '" + 
                 stringPrintf("%d", discuss_id) + "'";
  connection.connect();
  Result result_set= connection.excuteQuery(query);
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
    connection.close();
    return discuss;
  } else {
  	result_set.close();
    connection.close();
    return Discuss(0);
  }
}

DiscussList getDiscussList(const DiscussInfo& discuss_info){
  DiscussList discuss_list;
	bool first = true;
  MysqlConnection connection(host, user, password, database);
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
  connection.connect();
  Result result_set= connection.excuteQuery(query);
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
  connection.close();
  return discuss_list;
}

Error getError(int error_id){
	MysqlConnection connection(host, user, password, database);
  Error error(error_id);
  string query = "select * from errors where error_id = '" + 
                 stringPrintf("%d", error_id) + "'";
  connection.connect();
  Result result_set= connection.excuteQuery(query);
  if (result_set.next()){
  	error.setContent(result_set.getString("content"));
  	result_set.close();
    connection.close();
    return error;
  } else {
  	result_set.close();
    connection.close();
    return Error(0);
  }
}

Mail getMail(int mail_id){
  MysqlConnection connection(host, user, password, database);
  Mail mail(mail_id);
  string query = "select * from mails where mail_id = '" + 
                 stringPrintf("%d", mail_id) + "'";
  connection.connect();
  Result result_set= connection.excuteQuery(query);
  if (result_set.next()){
  	mail.setFromUser(result_set.getString("from_user"));
  	mail.setContent(result_set.getString("content"));
  	mail.setRead(result_set.getString("unread") != "Y");
  	mail.setTime(result_set.getString("time"));
  	mail.setTitle(result_set.getString("title"));
  	mail.setTopicId(result_set.getInt("topic_id"));
  	mail.setToUser(result_set.getString("to_user"));
  	result_set.close();
    connection.close();
    return mail;
  } else {
  	result_set.close();
    connection.close();
    return Mail(0);
  }
}

MailList getMailList(const MailInfo& mail_info){
  MailList mail_list;
	bool first = true;
  MysqlConnection connection(host, user, password, database);
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
  query += "order limit " + stringPrintf("%d, 25", mail_info.page_id*25);
  cout << query << endl;
  connection.connect();
  Result result_set= connection.excuteQuery(query);
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
  connection.close();
  return mail_list;
}

User getMostDiligenPlayer(){
	MysqlConnection connection(host, user, password, database);
  string query = "select user_id from statuses "
                 "group by user_id having count(*) >= ALL (select count(*) from statuses "
                 "group by user_id ) limit 0,1";
  //cout << query << endl;
  connection.connect();
  Result result_set = connection.excuteQuery(query);
  string user_id = "NULL";
  //cout << user_id << endl;
  if(result_set.next()){
  	user_id = result_set.getString("user_id");
  	cout << user_id << endl;
  }
  result_set.close();
  connection.close();
  return getUserInfo(user_id);
}

NewsList getNewsList(const NewsInfo& news_info){
	NewsList newslist;
  NewsListItem item;
  bool first = true;
  MysqlConnection connection(host, user, password, database);
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
  connection.connect();
  Result result_set= connection.excuteQuery(query);
  while(result_set.next()){
  	item.news_id = result_set.getInt("news_id");
  	item.title = result_set.getString("title");
  	item.time = result_set.getString("publishtime");
  	newslist.push_back(item);
  }
  result_set.close();
  connection.close();
  return newslist;
}

StatusList getNoSearchStatus(){
  StatusList statuslist;
  Status item;
  MysqlConnection connection(host, user, password, database);
  string query = "select * from statuses ";
  query += " order by submit_time desc ";
  query += " limit 0, 25";
  cout << query << endl;
  connection.connect();
  Result result_set= connection.excuteQuery(query);
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
  connection.close();
  return statuslist;
}
//
StatusList getSearchStatus(const StatusInfo& status_info){
  StatusList statuslist;
  Status item;
  bool first = true;
  MysqlConnection connection(host, user, password, database);
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
  connection.connect();
  Result result_set= connection.excuteQuery(query);
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
  connection.close();
  return statuslist;
}
/*
Notice getNotice();
*/
Problem getProblem(int problem_id){
  MysqlConnection connection(host, user, password, database);
  Problem problem(problem_id);
  string query = "select * from problems where problem_id = '" + 
                 stringPrintf("%d",problem_id) + "'";
  connection.connect();
  Result result_set= connection.excuteQuery(query);
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
    connection.close();
    return problem;
  } else {
  	result_set.close();
    connection.close();
    return Problem(0);
  }
}

ProblemList getProblemList(const ProblemInfo& problem_info){
	ProblemList problem_list;
	bool first = true;
  MysqlConnection connection(host, user, password, database);
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
    query += "problem_id in (select problem_id from problemtocontests where "; 
    query += "contest_id = '" + stringPrintf("%d", problem_info.related_contest) + 
             "') ";
  }
  
  query += " limit " + stringPrintf("%d, 25", problem_info.page_id*25);
  cout << query << endl;
  connection.connect();
  Result result_set= connection.excuteQuery(query);
  while(result_set.next()){
  	item.accepted = result_set.getInt("accepted");
  	item.problem_id = result_set.getInt("problem_id");
  	item.submit = result_set.getInt("submit");
  	item.title = result_set.getString("title");
  	problem_list.push_back(item);
  }
  result_set.close();
  connection.close();
  return problem_list;
}

RankList getRankList(const RankListInfo& ranklist_info){
  RankList ranklist;
  RankListItem item;
  bool first = true;
  MysqlConnection connection(host, user, password, database);
  string query = "select user_id, nickname, solveds, submits";
         query += " from users order by solveds, submits desc";
  query += " limit " + stringPrintf("%d, 25", ranklist_info.page_id*25);
  cout << query << endl;
  connection.connect();
  Result result_set= connection.excuteQuery(query);
  while(result_set.next()){
  	item.user_id = result_set.getString("user_id");
  	item.nickname = result_set.getString("nickname");
  	item.solved = result_set.getInt("solveds");
  	item.submit = result_set.getInt("submits");
  	ranklist.push_back(item);
  }
  result_set.close();
  connection.close();
  return ranklist;
}


ContestList getUpcomingContest(){
  MysqlConnection connection(host, user, password, database);
  ContestList contest_list;
  ContestListItem item;
  string now_time = getLocalTimeAsString("%Y-%m-%d %H:%M:%S");
  string query = "select * from contests where start_time > '";
         query += changeSymbol(now_time) + "' order by start_time";
  query += " limit 0,3";
  cout << query << endl;
  connection.connect();
  Result result_set= connection.excuteQuery(query);
  while(result_set.next()){
  	item.title = result_set.getString("title");
  	item.type = result_set.getString("contest_type");
  	item.start_time = result_set.getString("start_time");
  	item.end_time = result_set.getString("end_time");
  	item.contest_id = result_set.getInt("contest_id");
  	contest_list.push_back(item);
  }
  result_set.close();
  connection.close();
  return contest_list;
}

User getUserInfo(const string& user_id){
  MysqlConnection connection(host, user, password, database);
  User users(user_id);
  string query = "select * from users where user_id = '" + 
                 changeSymbol(user_id) + "'";
  cout << query << endl;
  connection.connect();
  Result result_set= connection.excuteQuery(query);
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
    connection.close();
    return users;
  } else {
  	result_set.close();
    connection.close();
    return User("NULL");
  }
}
//////////////////////////////////////////////// has bug
UserList getUserList(const UserInfo& user_info){
  UserList userlist;
  UserListItem item;
  bool first = true;
  MysqlConnection connection(host, user, password, database);
  string query = "select user_id, nickname, last_login_ip, last_login_time, ";
         query += "reg_time from users ";
  if (user_info.user_id != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "user_id = '" + changeSymbol(user_info.user_id) + 
             "' ";
  }
  if (user_info.nickname != string("NULL")){
    if (first){
      query += "where ";
      first = false;
    } else {
    	query += "and ";
    }
    query += "nickname like '%" + changeSymbol(user_info.nickname) + 
             "%' ";
  }
  query += " order by reg_time limit " + 
           stringPrintf("%d, 25", user_info.page_id*25);
  cout << query << endl;
  connection.connect();
  Result result_set= connection.excuteQuery(query);
  while(result_set.next()){
  	item.user_id = result_set.getString("user_id");
  	item.nickname = result_set.getString("nickname");
  	item.last_login_ip = result_set.getString("last_login_ip");
  	item.last_login_time = result_set.getString("last_login_time");
  	item.reg_time = result_set.getString("reg_time");
  	userlist.push_back(item);
  }
  result_set.close();
  connection.close();
  return userlist;
}
////////////////////////////////////////////////////////////// has bug

int main(){
	host = "localhost";
  user = "root";
  password = "noahoak";
  database = "onlinejudgetest";
  //cout << "Hello world" << endl;
  //Discuss discuss = getDiscuss(2);
  //cout << discuss.getTitle() << "---" << discuss.getContent() << "---" <<
  //        discuss.getUserId() << endl;
  //Error error = getError(2);
  //cout << error.getContent() << endl;
  Mail mail = getMail(2);
  cout << "Mail: " << mail.getTitle() << endl;
  cout << "to: " << mail.getToUser() << endl;
  cout << "from: " << mail.getFromUser() << endl;
  cout << mail.getContent() << endl; 
  //User user = getUserInfo("macheng");
  //cout << user.getSolved() << endl;
  //cout << user.getSubmit() << endl;
  //cout << user.getNickname() << endl;
  //Problem problem = getProblem(1);
  //cout << problem.getTitle() << " " <<problem.getDescription() << endl;
  ProblemInfo problem_info = {0, string("abc"), string("NULL"), 0, 0};
  RankListInfo ranklist_info = {0};
  UserInfo user_info = {string("NULL"),string("NULL"),0};
  DiscussInfo discuss_info = {"Apai's",1,1,string("NULL"),0};
  Contest contest(1);
  ContestInfo contestinfo = {"NULL", "Apai", 0};
  MailInfo mail_info = {"fu", "NULL", true, 0};
  NewsInfo news_info = {"NULL", 0};
  StatusInfo status_info = {"apai", 1, 0, 0, -1, true, true, 0};
  User user2 = getMostDiligenPlayer();
  cout << user2.getId() << " --- " << user2.getNickname() << endl;
  return 0;
}
