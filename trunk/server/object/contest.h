#ifndef _FLOOD_SERVER_DATA_CONTEST_H__
#define _FLOOD_SERVER_DATA_CONTEST_H__

#include <iostream>
#include <string>
using namespace std;

#include "list.h"

class Contest{
public:
  Contest(int contest_id = 0): contest_id_(contest_id){
  
  }
  Contest(int contest_id,
          int public_id,
          const string& title,
          const string& description,
          const string& start_time,
          const string& end_time,
          const string& type);
  
  int getContestId() const;
  int getPublicId() const;
  string getTitle() const;
  string getDescription() const;
  string getStartTime() const;
  string getEndTime() const;
  string getType() const;
  ContestListItem getContestListItem() const;
  
  void setContestId(int contest_id);
  void setPublicId(int public_id);
  void setTitle(const string& title);
  void setDescription(const string& description);
  void setStartTime(const string& start_time);
  void setEndTime(const string& end_time);
  void setType(const string& type);
  
private:
  int contest_id_;
  int public_id_;
  string title_;
  string description_;
  string start_time_;
  string end_time_;
  string type_;
};

#endif /*CONTEST_H_*/
