#ifndef _FLOOD_SERVER_DATA_DATAINTERFACE_H__
#define _FLOOD_SERVER_DATA_DATAINTERFACE_H__

#include <string>

#include "object/objectinc.h"
#include "databaseinterface.h"
#include "fileinterface.h"
#include "cachemanager.h"
using namespace std;

class DataInterface{
public:
  DataInterface(){
  }

  int addContest(const Contest& contest);
  int addProblemListtoContest(const Contest& contest,const ProblemSet& problem_set);
  int addUserListtoContest(const Contest& contest,const UserSet& user_set);
  int addNews(const News& news);
  //int addNotice(const Notice& notice);
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
  int addInputtoOutput(int in_id, int out_id);
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
  UserList getMostDiligenPlayer();
  NewsList getNewsList(const NewsInfo& news_info);
  StatusList getNoSearchStatus();
  StatusList getSearchStatus(const StatusInfo& status_info);
  //Notice getNotice();
  Problem getProblem(int problem_id);
  ProblemList getProblemList(const ProblemInfo& problem_info);
  RankList getRankList(const RankListInfo& ranklist_info);
  ContestList getUpcomingContest();
  User getUserInfo(const string& user_id);
  UserList getUserList(const UserInfo& user_info);
  DiscussList getReplyDiscussList(int message_id);
  TopicSet getDiscussTopicSet(const DiscussInfo& discuss_info);
  int updateProblem(const Problem& problem);
  int updateContest(const Contest& contest);
  //int updateProblemListtoContest(const Contest& constest,const ProblemList& problem_list);
  //int updateUserListtoContest(const Contest& contest,const UserList& user_list);
  int updateDiscuss(const Discuss& discuss);
  int updateStatus(const Status& status);
  int updateUser(const User& user);
  int updateNews(const News& news);
  int updateError(const Error& error);
  //int updateCode(const Code& code);
  ProblemStatistics getProblemStatistics(int problem_id);
  int addUser(const User& user);
  int setCodeSharing(const Code& code);
  int disableProblem(const Problem& problem);
  int disableUser(const User& user);
  int disableContest(const Contest& contest);
  bool checkContestAcBefore(const ContestAcBefore&);
  int getInContestId(int contest_id, int problem_id);
  int disableContestProblems(const Contest& contest);
  int disableContestUsers(const Contest& contest);
  bool checkPermission(int contest_id, const string& user_id);
  ContestProblemList getContestProblemList(int contest_id);
  int getContestProblemNum(int contest_id);
  int disableMail(const string& user_id, int mail_id);
  int setMailRead(const Mail& mail);
  ProblemSet getUserACProblem(const string& user_id, bool ac);
  StatusList getProblemStatus(const StatusInfo& status_info);
  int getUserRank(const string& user_id);
  map<string, string> getProblemInAndOutFile(const Problem& problem);
  string getProblemSpjFile(const Problem& problem);
  Status getStatus(int status_id);
  int getProblemListNum();
  int getContestListNum(bool normal);
  
  int addLink(const LinkList& link_list);
  int addFile(const string& filename, void* bufi, size_t filelength);
  FileData getFile(const string& filename);
  int updateFile(const string& filename, void* buf, size_t filelength);
  int updateLink(const LinkList& link_list);
  LinkList getLink();
  int updateNotice(const string& notice, const string& time);
  string getNotice();
  ssize_t getFileSize(const string& filename);
  int updateFileVersion(int problem_id, int contest_id);

  static DataInterface& getInstance(){
    if(instance == NULL)
      instance = new DataInterface;
    return *instance;
  }
  static void destroy(){
    if(instance)
      delete instance;
    instance = NULL;
  }
private:
  static DataInterface* instance;
};

#endif

