#ifndef _FLOOD_SERVER_DATA_MAIL_H__
#define _FLOOD_SERVER_DATA_MAIL_H__

#include <string>
using namespace std;

#include "list.h"

class Mail{
public:
  Mail(int mail_id) : mail_id_(mail_id){
  
  }
  Mail(int mail_id,
	     int topic_id,
	     const string& to_user,
	     const string& from_user,
	     const string& title,
	     const string& content,
       bool read,
	     const string& time);
	     
  int getMailId() const;
  int getTopicId() const;
  string getToUser() const;
  string getFromUser() const;
  string getTitle() const;
  string getContent() const;
  bool getRead() const;
  string getTime() const;  
  void setMailId(int mail_id);
  void setTopicId(int topic_id);
  void setToUser(const string& to_user);
  void setFromUser(const string& from_user);
  void setTitle(const string& title);
  void setContent(const string& content);
  void setRead(bool read);
  void setTime(const string& time);
  MailListItem getMailListItem() const;
  
private:
	int mail_id_;
	int topic_id_;
	string to_user_;
	string from_user_;
	string title_;
	string content_;
  bool read_;
	string time_;
};

#endif /*MAIL_H_*/
