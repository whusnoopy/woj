#include "course.h"

Course::Course(int course_id, 
               const string& description, 
               const string& teacher, 
               bool available) 
  : course_id_(course_id),
    description_(description),
    user_id_(teacher),
    available_(available){
} 
  
int Course::getCourseId() const{
  return course_id_;
}

string Course::getDescription() const {
  return description_;
}

string Course::getTeacher() const {
  return user_id_;
}

bool Course::getAvailable() const {
  return available_;
}

void Course::setCourseId(int course_id) {
  course_id_ = course_id;
}

void Course::setDescription(const string& description) {
  description_ = description;
}

void Course::setTeacher(const string& user_id) {
  user_id_ = user_id;
}

void Course::setAvailable(bool available){
  available_ = available;
}


