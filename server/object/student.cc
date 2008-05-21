#include "student.h"

Student::Student(const string& user_id,
                 const string& student_id,
                 const string& realname,
                 const Class& mclass,
                 bool available) 
 : user_id_(user_id),
   student_id_(student_id),
   realname_(realname),
   mclass_(mclass),
   available_(available){
}

string Student::getUserId() const {
  return user_id_;
}

string Student::getStudentId() const {
  return student_id_;
}

string Student::getRealName() const {
  return realname_;
}

Class Student::getClass() const {
  return mclass_;
}

bool Student::getAvailable() const {
  return available_;
}

void Student::setUserId(const string& user_id) {
  user_id_ =  user_id;
}

void Student::setStudentId(const string& student_id) {
  student_id_ = student_id;
}

void Student::setRealName(const string& realname) {
  realname_ = realname;
}

void Student::setClass(const Class& m_class) {
  mclass_ = m_class;
}

void Student::setAvailable(bool available) {
  available_ = available;
}

