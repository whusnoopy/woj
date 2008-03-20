#include "mail.h"

Mail::Mail(int mail_id,
           int topic_id,
           const string& to_user,
           const string& from_user,
           const string& title,
           const string& content,
           bool read,
           const string& time)
: mail_id_(mail_id),
  topic_id_(topic_id),
  to_user_(to_user),
  from_user_(from_user),
  title_(title),
  content_(content),
  read_(read),
  time_(time){
  
}
           
           
int Mail::getMailId() const{
  return mail_id_;
}

int Mail::getTopicId() const{
  return topic_id_;
}

string Mail::getToUser() const{
  return to_user_;
}

string Mail::getFromUser() const{
  return from_user_;
}

string Mail::getTitle() const{
  return title_;
}

string Mail::getContent() const{
  return content_;
}

bool Mail::getRead() const{
  return read_;
}

string Mail::getTime() const{
  return time_;
}
 
void Mail::setMailId(int mail_id){
  mail_id_ = mail_id;
}

void Mail::setTopicId(int topic_id){
  topic_id_ = topic_id;
}

void Mail::setToUser(const string& to_user){
  to_user_ = to_user;
}

void Mail::setFromUser(const string& from_user){
  from_user_ = from_user;
}

void Mail::setTitle(const string& title){
  title_ = title;
}

void Mail::setContent(const string& content){
  content_ = content;
}

void Mail::setRead(bool read){
  read_ = read;
}

void Mail::setTime(const string& time){
  time_ = time;
}

MailListItem Mail::getMailListItem() const{
  MailListItem item;
  item.date = time_;
  item.from_user = from_user_;
  item.mail_id = mail_id_;
  item.title = title_;
  item.to_user = to_user_;
  return item;
}
