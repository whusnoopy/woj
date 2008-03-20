#ifndef _FLOOD_SERVER_DATA_NEWS_H__
#define _FLOOD_SERVER_DATA_NEWS_H__

#include <string>
using namespace std;

#include "list.h"

class News{
public:
  News(int news_id = 0) : news_id_(news_id){
  }
  News(int news_id,
	     const string& publish_time,
	     const string& title,
	     const string& content);
	
	int getNewsId() const;
	string getPublishTime() const;
	string getTitle() const;
	string getContent() const;
	void setNewId(int news_id);
	void setPublishTime(const string& publish_time);
	void setTitle(const string& title);
	void setContent(const string& content);
	NewsListItem getNewsListItem() const;

private:
  int news_id_;
	string publish_time_;
	string title_;
	string content_;
};

#endif /*NEWS_H_*/
