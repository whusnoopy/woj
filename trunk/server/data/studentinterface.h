#ifndef _FLOOD_SERVER_DATA_STUDENTINTERFACE_H__
#define _FLOOD_SERVER_DATA_STUDENTINTERFACE_H__

#include <string>

#include "server/object/student.h"
#include "server/data/connectionfactory.h"

using namespace std;

class StudentInterface: public ConnectionFactory{
public:
  StudentInterface() {
  }
  
  ~StudentInterface() {
  }

  int addStudent(const Student&);
  int updateStudent(const Student&);
  int disableStudent(const string& user_id, bool available);
  StudentList getStudentList(int grade);
  StudentList getStudentList(int grade, int class_no);
  Student getStudent(const string& user_id);
  bool isStudent(const string& user_id);
  int addStudentToCourse(const string& user_id, int course_id);
  int deleteStudentFromCourse(const string& user_id, int course_id);

  static StudentInterface& getInstance(){
    if(instance == NULL)
      instance = new StudentInterface;
    return *instance;
  }

  static void destroy(){
    if(instance)
      delete instance;
    instance = NULL;
  }
private:
  static StudentInterface * instance;
  
};


#endif
