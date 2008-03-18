#include "news.h"

News::News(int news_id,
	         const string& publish_time,
	         const string& title,
	         const string& content)
: news_id_(news_id),
  publish_time_(publish_time),
  title_(title),
  content_(content){

} 
	
int News::getNewsId() const{
  return news_id_;
}

string News::getPublishTime() const{
  return publish_time_;
}

string News::getTitle() const{
  return title_;
}

string News::getContent() const{
  return content_;
}

void News::setNewId(int news_id){
  news_id_ = news_id;  
}

void News::setPublishTime(const string& publish_time){
  publish_time_ = publish_time;
}

void News::setTitle(const string& title){
  title_ = title;
}

void News::setContent(const string& content){
  content_ = content;
}

NewsListItem News::getNewsListItem() const{
  NewsListItem item;
  item.news_id = news_id_;
  item.title = title_;
}
