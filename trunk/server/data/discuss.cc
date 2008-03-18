#include "discuss.h"


Discuss::Discuss(int message_id,
                 int reply_id,
                 int topic_id,
                 const string& user_id,
                 int problem_id,
                 int contest_id,
                 const string& tilte,
                 const string& content,
                 const string& date,
                 bool available)
: message_id_(message_id),
  reply_id_(reply_id),
  topic_id_(topic_id),
  user_id_(user_id),
  problem_id_(problem_id),
  contest_id_(contest_id),
  title_(tilte),
  content_(content),
  date_(date),
  available_(available){
}
    
int Discuss::getMessageId() const {
  return message_id_;
}

int Discuss::getReplyId() const {
  return reply_id_;
}

int Discuss::getTopicId() const {
  return topic_id_;
}

string Discuss::getUserId() const {
  return user_id_;
}

int Discuss::getProblemId() const {
  return problem_id_;
}

int Discuss::getContestId() const {
  return contest_id_;
}

string Discuss::getTitle() const {
  return title_;
}

string Discuss::getContent() const {
  return content_;
}

bool Discuss::getAvailable() const {
  return available_;
}
          

void Discuss::setMessageId(int message_id){
  message_id_ = message_id;
}

void Discuss::setReplyId(int reply_id){
  reply_id_ = reply_id;
}

void Discuss::setTopicId(int topic_id){
  topic_id_ = topic_id;
}

void Discuss::setUserId(const string& user_id){
  user_id_ = user_id;
}

void Discuss::setProblemId(int problem_id){
  problem_id_ = problem_id;
}

void Discuss::setContestId(int contest_id){
  contest_id_ = contest_id;
}

void Discuss::setTilte(const string& title){
  title_ = title;
}

void Discuss::setContent(const string& content){
  content_ = content;
}

void Discuss::setAvailable(bool available){
  available_ = available;
}

DiscussListItem Discuss::getDiscussListItem() const {
  DiscussListItem item;
  item.contest_id = contest_id_;
  item.date = date_;
  item.discuss_id = message_id_;
  item.reply_id = reply_id_;
  item.topic_id = reply_id_;
  item.user_id = user_id_;
  item.problem_id = problem_id_;
  item.title = title_;
  return item;
}

string Discuss::getDate() const {
  return date_;
}

void Discuss::setDate(const string& date){
  date_ = date;
}

