#ifndef _FLOOD_SERVER_DATA_DISCUSS_H__
#define _FLOOD_SERVER_DATA_DISCUSS_H__

#include <string>
using namespace std;

#include "list.h"

class Discuss{
public:
	Discuss(int message_id = 0) : message_id_(message_id){
	}
	Discuss(int message_id,
	        int reply_id,
	        int topic_id,
	        const string& user_id,
	        int problem_id,
	        int contest_id,
	        const string& tilte,
	        const string& content,
	        const string& date,
	        bool available);
  
  int getMessageId() const;
	int getReplyId() const;
	int getTopicId() const;
	string getUserId() const;
	int getProblemId() const;
	int getContestId() const;
	string getTitle() const;
	string getContent() const;	        
  DiscussListItem getDiscussListItem() const;
  string getDate() const;
  bool getAvailable() const;

  void setMessageId(int message_id);
	void setReplyId(int reply_id);
	void setTopicId(int topic_id);
	void setUserId(const string& user_id);
	void setProblemId(int problem_id);
	void setContestId(int contest_id);
	void setTitle(const string& title);
	void setContent(const string& content);
  void setDate(const string& date);
  void setAvailable(bool available);
  
private:
  int message_id_;
	int reply_id_;
	int topic_id_;
	string user_id_;
	string date_;
	int problem_id_;
	int contest_id_;
	string title_;
	string content_;
	bool available_;
};

#endif /*DISCUSS_H_*/
