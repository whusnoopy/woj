#include "teacher.h"

Teacher::Teacher(const string& user_id, 
                 const string& realname,
                 bool available)
  : user_id_(user_id), 
    realname_(realname),
    available_(available){
}

string Teacher::getUserId() const {
  return user_id_;
}

string Teacher::getRealName() const {
  return realname_;
}

bool Teacher::getAvailable() const {
  return available_;
}
  
void Teacher::setUserId(const string& user_id) {
  user_id_ = user_id;
}

void Teacher::setRealName(const string& realname) {
  realname_ = realname;
}

void Teacher::setAvailable(bool available) {
  available_ = available;
}
