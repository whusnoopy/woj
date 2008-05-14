#include "class.h"

Class::Class(const string& college, int grade, class_number) 
  : college_(college), grade_(grade), class_number_(class_number){
}

string Class::getCollege() const {
  return college_;
}

int Class::getGrade() const {
  return grade_;
}

int Class::getClass() const {
  return class_number_;
}

void Class::setCollege(const string& college) {
  college_ = college;
}

void Class::setGrade(int grade) {
  grade_ = grade;
}

void Class::setClass(int class_number) {
  class_number_ = class_number;
}
