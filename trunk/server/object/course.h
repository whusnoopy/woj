#ifndef _FLOOD_SERVER_DATA_COURSE_H__
#define _FLOOD_SERVER_DATA_COURSE_H__

#include <string>
#include <vector>
using namespace std;

class Course{
public:
  Course(int course_id = '0') {
  
  }

  Course(int course_id, const string& description, const string& teacher, bool available);
  
  int getCourseId() const;
  string getDescription() const;
  string getTeacher() const;
  bool getAvailable() const;
  
  void setCourseId(int);
  void setDescription(const string&); 
  void setTeacher(const string&);
  void setAvailable(bool available);

private:
  int course_id_;
  string description_;
  string user_id_;
  bool available_;
};

typedef struct _COURSE_ITEM_ {
  int course_id;
  string description;
  string teacher;
  bool available;
}CourseItem;

typedef vector<CourseItem> CourseList;
#endif /*JOB_H_*/
