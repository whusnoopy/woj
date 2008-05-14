#ifndef _FLOOD_SERVER_DATA_TEACHER_H__
#define _FLOOD_SERVER_DATA_TEACHER_H__

#include <string>
using namespace std;

class Teacher{
public:
  Teacher() {
  }
  
  Teacher(const string&, const string&, bool);
  string getUserId() const;
  string getRealName() const;
  bool getAvailable() const;
  
  void setUserId(const string&);
  void setRealName(const string&);
  void setAvailable(bool available);  

private:
  string user_id_;
  string realname_;
  bool available_;
};

#endif /*TEACHER_H_*/
