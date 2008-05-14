#ifndef _FLOOD_SERVER_DATA_CLASS_H__
#define _FLOOD_SERVER_DATA_CLASS_H__

#include <string>
using namespace std;

class Class{
public:
  Class(const string& college = "NULL", int grade = 2000, class_number = 0);
  string getCollege() const;
  int getGrade() const;
  int getClass() const;

  void setCollege(const string& college);
  void setGrade(int grade);
  void setClass(int class_number);
   
private:
  string college_;
  int grade_;
  int class_number_;
};

#endif /*Class_H_*/
