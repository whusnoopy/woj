#include "datainterface.h"

DataInterface* DataInterface::instance = NULL;

int DataInterface::addContest(const Contest& contest) {
  return DatabaseInterface::getInstance().addContest(contest);
}

int DataInterface::addProblemListtoContest(const Contest& contest,
                                           const ProblemIdList& problem_list) {
  return DatabaseInterface::getInstance().addProblemListtoContest(contest, problem_list);
}

int DataInterface::addUserListtoContest(const Contest& contest,
                                        const UserSet& user_set) {
  return DatabaseInterface::getInstance().addUserListtoContest(contest, user_set);
}

int DataInterface::addNews(const News& news) {
  return DatabaseInterface::getInstance().addNews(news);
}

  //int addNotice(const Notice& notice);
int DataInterface::addProblem(const Problem& problem) {
  return DatabaseInterface::getInstance().addProblem(problem);
}
  
int DataInterface::addStatus(const Status& status) {
  return DatabaseInterface::getInstance().addStatus(status);
}
  
int DataInterface::addMail(const Mail& mail) {
  return DatabaseInterface::getInstance().addMail(mail);
}

Status DataInterface::getStatus(int status_id) {
  return DatabaseInterface::getInstance().getStatus(status_id);
}

int DataInterface::addCode(const Code& code) {
  return DatabaseInterface::getInstance().addCode(code);
}

int DataInterface::addError(const Error& error) {
  return DatabaseInterface::getInstance().addError(error);
}

int DataInterface::addDiscuss(const Discuss& discuss) {
  return DatabaseInterface::getInstance().addDiscuss(discuss);
}

int DataInterface::deleteDiscuss(const Discuss& discuss) {
  return DatabaseInterface::getInstance().deleteDiscuss(discuss);
}

int DataInterface::disableDiscuss(const Discuss& discuss) {
  return DatabaseInterface::getInstance().disableDiscuss(discuss);
}

int DataInterface::deleteError(const Error& error) {
  return DatabaseInterface::getInstance().deleteError(error);
}

int DataInterface::addFilePathtoProblem(const File& file, 
                                        const Problem& problem) {
  return DatabaseInterface::getInstance().addFilePathtoProblem(file, problem);
}

int DataInterface::addFilePathtoContest(const File& file, 
                                        const Contest& contest) {
  return DatabaseInterface::getInstance().addFilePathtoContest(file, contest);
}

int DataInterface::addInputtoOutput(int in_id, int out_id) {
  return DatabaseInterface::getInstance().addInputtoOutput(in_id, out_id);
}

FileList DataInterface::getProblemFile(const Problem& problem) {
  return DatabaseInterface::getInstance().getProblemFile(problem);
}

FileList DataInterface::getContestFile(const Contest& contest) {
  return DatabaseInterface::getInstance().getContestFile(contest);
}

Code DataInterface::getCode(int code_id) {
  return DatabaseInterface::getInstance().getCode(code_id);
}

Contest DataInterface::getContest(int contest_id) {
  return DatabaseInterface::getInstance().getContest(contest_id);
}

ContestList DataInterface::getContestList(const ContestInfo& contest_info) {
  return DatabaseInterface::getInstance().getContestList(contest_info);
}

ContestRankList DataInterface::getContestRankList(const ContestRankListInfo& contest_ranklist_info) {
  ContestRankList contest_ranklist = CacheManager::getInstance().getContestRankList(contest_ranklist_info.contest_id);
  //return the $page_id page
  return contest_ranklist;
}

int DataInterface::updateFileVersion(int problem_id, int contest_id) {
  return DatabaseInterface::getInstance().updateFileVersion(problem_id, contest_id);
}

ContestStatistics DataInterface::getContestStatistics(int contest_id) {
  return CacheManager::getInstance().getContestStatistics(contest_id);
}

Discuss DataInterface::getDiscuss(int discuss_id) {
  return DatabaseInterface::getInstance().getDiscuss(discuss_id);
}

DiscussList DataInterface::getDiscussList(const DiscussInfo& discuss_info) {
  return DatabaseInterface::getInstance().getDiscussList(discuss_info);
}

Error DataInterface::getError(int error_id) {
  return DatabaseInterface::getInstance().getError(error_id);
}

int DataInterface::disableContestProblems(const Contest& contest) {
  return DatabaseInterface::getInstance().disableContestProblems(contest);
}

int DataInterface::disableContestUsers(const Contest& contest) {
  return DatabaseInterface::getInstance().disableContestUsers(contest);
}

bool DataInterface::checkPermission(int contest_id, const string& user_id) {
  return DatabaseInterface::getInstance().checkPermission(contest_id, user_id);
}

ContestProblemList DataInterface::getContestProblemList(int contest_id) {
  return DatabaseInterface::getInstance().getContestProblemList(contest_id);
}

ProblemIdList DataInterface::getContestProblems(int contest_id) {
  return DatabaseInterface::getInstance().getContestProblems(contest_id);
}

int DataInterface::getContestProblemNum(int contest_id) {
  return DatabaseInterface::getInstance().getContestProblemNum(contest_id);
}

int DataInterface::disableMail(const string& user_id, int mail_id) {
  return DatabaseInterface::getInstance().disableMail(user_id, mail_id);
}
 
Mail DataInterface::getMail(int mail_id) {
  return DatabaseInterface::getInstance().getMail(mail_id);
}

MailList DataInterface::getMailList(const MailInfo& mail_info) {
  return DatabaseInterface::getInstance().getMailList(mail_info);
}

UserList DataInterface::getMostDiligenPlayer() {
  return DatabaseInterface::getInstance().getMostDiligenPlayer();
}

int DataInterface::setMailRead(const Mail& mail) {
  return DatabaseInterface::getInstance().setMailRead(mail);
}

NewsList DataInterface::getNewsList(const NewsInfo& news_info) {
  return DatabaseInterface::getInstance().getNewsList(news_info);
}

StatusList DataInterface::getNoSearchStatus() {
  return CacheManager::getInstance().getStatus();
}

StatusList DataInterface::getSearchStatus(const StatusInfo& status_info) {
  return DatabaseInterface::getInstance().getSearchStatus(status_info);
}

  //Notice getNotice();
Problem DataInterface::getProblem(int problem_id) {
  return DatabaseInterface::getInstance().getProblem(problem_id);
}

ProblemList DataInterface::getProblemList(const ProblemInfo& problem_info) {
  return DatabaseInterface::getInstance().getProblemList(problem_info);
}

RankList DataInterface::getRankList(const RankListInfo& ranklist_info) {
  return DatabaseInterface::getInstance().getRankList(ranklist_info);
}

ProblemSet DataInterface::getUserACProblem(const string& user_id, bool ac) {
  return DatabaseInterface::getInstance().getUserACProblem(user_id, ac);
}

StatusList DataInterface::getProblemStatus(const StatusInfo& status_info) {
  return DatabaseInterface::getInstance().getProblemStatus(status_info);
}

int DataInterface::getUserRank(const string& user_id) {
  return DatabaseInterface::getInstance().getUserRank(user_id);
}

ContestList DataInterface::getUpcomingContest(){
  return DatabaseInterface::getInstance().getUpcomingContest();
}

User DataInterface::getUserInfo(const string& user_id) {
  return DatabaseInterface::getInstance().getUserInfo(user_id);
}

UserList DataInterface::getUserList(const UserInfo& user_info) {
  return DatabaseInterface::getInstance().getUserList(user_info);
}

DiscussList DataInterface::getReplyDiscussList(int message_id) {
  return DatabaseInterface::getInstance().getReplyDiscussList(message_id);
}

TopicSet DataInterface::getDiscussTopicSet(const DiscussInfo& discuss_info) {
  return DatabaseInterface::getInstance().getDiscussTopicSet(discuss_info);
}

int DataInterface::updateProblem(const Problem& problem) {
  return DatabaseInterface::getInstance().updateProblem(problem);
}

int DataInterface::updateContest(const Contest& contest) {
  return DatabaseInterface::getInstance().updateContest(contest);
}
/*
int DataInterface::updateProblemListtoContest(const Contest& contest,
                                              const ProblemList& problem_list) {
  return DatabaseInterface::getInstance().updateProblemListtoContest(contest, problem_list);
}

int DataInterface::updateUserListtoContest(const Contest& contest,
                                           const UserList& user_list) {
  return DatabaseInterface::getInstance().updateUserListtoContest(contest, user_list);
}
*/
int DataInterface::updateDiscuss(const Discuss& discuss) {
  return DatabaseInterface::getInstance().updateDiscuss(discuss);
}

int DataInterface::updateStatus(const Status& status) {
  //chinese :tong bu cache
  return CacheManager::getInstance().updateStatus(status);
}

int DataInterface::updateUser(const User& user) {
  return DatabaseInterface::getInstance().updateUser(user);
}

int DataInterface::updateNews(const News& news) {
  return DatabaseInterface::getInstance().updateNews(news);
}

int DataInterface::updateError(const Error& error) {
  return DatabaseInterface::getInstance().updateError(error);
}

//int updateCode(const Code& code);

ProblemStatistics DataInterface::getProblemStatistics(int problem_id) {
  return DatabaseInterface::getInstance().getProblemStatistics(problem_id);
}

int DataInterface::addUser(const User& user) {
  return DatabaseInterface::getInstance().addUser(user);
}

map<string, string> DataInterface::getProblemInAndOutFile(const Problem& problem) {
  return DatabaseInterface::getInstance().getProblemInAndOutFile(problem);
}

string DataInterface::getProblemSpjFile(const Problem& problem) {
  return DatabaseInterface::getInstance().getProblemSpjFile(problem);
}

int DataInterface::setCodeSharing(const Code& code) {
  return DatabaseInterface::getInstance().setCodeSharing(code);
}

int DataInterface::disableProblem(const Problem& problem) {
  return DatabaseInterface::getInstance().disableProblem(problem);
}

int DataInterface::disableUser(const User& user) {
  return DatabaseInterface::getInstance().disableUser(user);
}

int DataInterface::disableContest(const Contest& contest) {
  return DatabaseInterface::getInstance().disableContest(contest);
}

bool DataInterface::checkContestAcBefore(const ContestAcBefore& contest_acbefore) {
  return DatabaseInterface::getInstance().checkContestAcBefore(contest_acbefore);
}

int DataInterface::updateUserSolved(const Status& status, int op) {
  return DatabaseInterface::getInstance().updateUserSolved(status, op);
}

int DataInterface::getInContestId(int contest_id, int problem_id) {
  return DatabaseInterface::getInstance().getInContestId(contest_id, problem_id);
}

StatusList DataInterface::getClientStatusList(const string& user_id) {
  return DatabaseInterface::getInstance().getClientStatusList(user_id);
}

ProblemIdList DataInterface::getClientProblemList() {
  return DatabaseInterface::getInstance().getClientProblemList();
}

ContestInfoList DataInterface::getClientContestList() {
  return DatabaseInterface::getInstance().getClientContestList();
}

int DataInterface::addLink(const LinkList& link_list) {
  return FileInterface::getInstance().addLink(link_list);
}

int DataInterface::addFile(const string& filename, void* bufi, size_t filelength) {
  return FileInterface::getInstance().addFile(filename, bufi, filelength);
}

FileData DataInterface::getFile(const string& filename) {
  return CacheManager::getInstance().getFileData(filename);
}

int DataInterface::getProblemListNum(const ProblemInfo& problem_info) {
  return DatabaseInterface::getInstance().getProblemListNum(problem_info);
}
int DataInterface::getContestListNum(bool normal) {
  return DatabaseInterface::getInstance().getContestListNum(normal);
}
 
int DataInterface::updateFile(const string& filename, void* buf, size_t filelength) {
  return FileInterface::getInstance().updateFile(filename, buf, filelength);
}

int DataInterface::updateLink(const LinkList& link_list) {
  return FileInterface::getInstance().updateLink(link_list);
}

LinkList DataInterface::getLink() {
  return FileInterface::getInstance().getLink();
}

int DataInterface::updateNotice(const string& notice, const string& time) {
  return FileInterface::getInstance().updateNotice(notice, time);
}

string DataInterface::getNotice() {
  return FileInterface::getInstance().getNotice();
}

ssize_t DataInterface::getFileSize(const string& filename) {
  return FileInterface::getInstance().getFileSize(filename);
}


