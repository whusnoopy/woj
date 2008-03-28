#include "contest.h"

Contest::Contest(int contest_id,
                 int public_id,
                 const string& title,
                 const string& description,
                 const string& start_time,
                 const string& end_time,
                 const string& type,
                 int version,
                 bool available)
: contest_id_(contest_id),
  public_id_(public_id),
  title_(title),
  description_(description),
  start_time_(start_time),
  end_time_(end_time),
  type_(type),
  version_(version),
  available_(available){
}
  
int Contest::getContestId() const{
   return contest_id_;
}
 
int Contest::getPublicId() const{
   return public_id_;
}
 
string Contest::getTitle() const{
   return title_;
}
 
string Contest::getDescription() const{
   return description_;
}
 
string Contest::getStartTime() const{
   return start_time_;
}
 
string Contest::getEndTime() const{
   return end_time_;
}
 
string Contest::getType() const{
   return type_;
}

int Contest::getVersion() const {
  return version_;
}

bool Contest::getAvailable() const {
  return available_;
}
 
  
void Contest::setContestId(int contest_id){
  contest_id_ = contest_id; 
}
 
void Contest::setPublicId(int public_id){
  public_id_ = public_id;
}
 
void Contest::setTitle(const string& title){
  title_ = title;
}
 
void Contest::setDescription(const string& description){
  description_ = description;
}
 
void Contest::setStartTime(const string& start_time){
  start_time_ = start_time;
}
 
void Contest::setEndTime(const string& end_time){
  end_time_ = end_time;
}
 
void Contest::setType(const string& type){
  type_ = type;
}

void Contest::setVersion(int version) {
  version_ = version;
}

void Contest::setAvailable(bool available) {
  available_ = available;
}

ContestListItem Contest::getContestListItem() const{
  ContestListItem item;
  item.title = title_;
  item.end_time = end_time_;
  item.start_time = start_time_;
  item.type = type_;
  return item;
}

 
