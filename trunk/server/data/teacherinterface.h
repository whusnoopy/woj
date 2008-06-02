#ifndef _FLOOD_SERVER_DATA_TEACHERINTERFACE_H__
#define _FLOOD_SERVER_DATA_TEACHERINTERFACE_H__

#include <string>

#include "object/teacher.h"
#include "object/course.h"
#include "object/list.h"
#include "data/connectionfactory.h"
using namespace std;

class TeacherInterface: public ConnectionFactory{
public:
  TeacherInterface() {
  }
  
  ~TeacherInterface() {
  }

  int addTeacher(const Teacher&);
  int updateTeacher(const Teacher&);
  int disableTeacher(const string& user_id, bool available);
  int addControlClass(const string& user_id, const string& description);
  int deleteControlClass(const string& user_id, int course_id);
  TeacherList getTeacherList();
  Teacher getTeacher(const string& user_id);
  bool isTeacher(const string& user_id);
  CourseList getCourseList(const string& user_id);
  Course getCourse(int course_id);
  int setStatusResult(int status_id, int result);
  StatusList getStatusListForTeacher(int job_id, int page_id);

  static TeacherInterface& getInstance(){
    if(instance == NULL)
      instance = new TeacherInterface;
    return *instance;
  }

  static void destroy(){
    if(instance)
      delete instance;
    instance = NULL;
  }
private:
  static TeacherInterface * instance;
  
};


#endif
