#ifndef _FLOOD_SERVER_DATA_DATABASEINTERFACE_H__
#define _FLOOD_SERVER_DATA_DATABASEINTERFACE_H__

#include <string>
using namespace std;

class DatabaseInterface{
public:
  DatabaseInterface(){
  }

  int addContest(const Contest& contest);
  int addProblemListtoContest(const Contest& contest,const ProblemList& problem_list);
  int addUserListtoContest(const Contest& contest,const UserList& user_list);
  int addNews(const News& news);
  int addNotice(const Notice& notice);
  int addProblem(const Problem& problem);
  int addStatus(const Status& stautus);
  int addMail(const Mail& mail);
  int addCode(const Code& code);
  int addError(const Error& error);
  int addDiscuss(const Discuss& discuss);
  int deleteDiscuss(const Discuss& discuss);
  int disableDiscuss(const Discuss& discuss);
  int deleteError(const Error& error);
  int addFilePathtoProblem(const File& file, const Problem& problem);
  int addFilePathtoContest(const File& file, const Contest& contest);
  FileList getProblemFile(const Problem& problem);
  FileList getContestFile(const Contest& contest);
  Code getCode(int code_id);
  Contest getContest(int contest_id);
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
  User getUserInfo(const string& user_id);
  UserList getUserList(const UserInfo& user_info);
  int updateProblem(const Problem& problem);
  int updateContest(const Contest& contest);
  int updateProblemListtoContest(const Contest& constest,const ProblemList& problem_list);
  int updateUserListtoContest(const Contest& contest,const UserList& user_list);
  int updateDiscuss(const Discuss& discuss);
  int updateStatus(const Status& status);
  int updateUser(const User& user);
  int updateNews(const News& news);
  int updateError(const Error& error);
  //int updateCode(const Code& code);
  ProblemStatitics getProblemStatitics(int problem_id);
  int addUser(const User& user);
  int setCodeSharing(const Code& code);
  int disableProblem(const Problem& problem);
  int disableUser(const User& user);
  Connection createConnection(const string& host,
                              const string& user,
                              const string& password,
                              const string& database);
  Connection createConnection();

  static DatabaseInterface& getInstance(){
    if(instance == NULL)
      instance = new DatabaseInterface;
    return *instance;
  }
  static void destory(){
    if(instance)
      delete instance;
    instance = NULL;
  }
private:
  static DatabaseInterface* instance;
};

#endif
