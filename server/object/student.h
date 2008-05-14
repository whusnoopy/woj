#ifndef _FLOOD_SERVER_DATA_STUDENT_H__
#define _FLOOD_SERVER_DATA_STUDENT_H__

#include <string>

#include "object/class.h"
using namespace std;

class Student{
public:
  Student() [
  }
  
  Student(const string& user_id,
          const string& student_id,
          const string& realname,
          const Class& mclass,
          bool available);

  string getUserId() const;
  string getStudentId() const;
  string getRealName() const;
  Class getClass() const;
  bool getAvailable() const;

  void setUserId(const string& user_id);
  void setStudentId(const string& student_id);
  void setRealName(const string& realname);
  void setClass(const Class& m_class);
  void setAvailable(bool available);
   
private:
  string user_id_;
  string realname_;
  string student_id_;
  bool avaiable_;
  Class mclass_;
};

#endif /*_STUDENT_H_*/
