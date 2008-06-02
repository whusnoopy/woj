#ifndef _FLOOD_SERVER_DATA_TEACHINTERFACE_H__
#define _FLOOD_SERVER_DATA_TEACHINTERFACE_H__

#include <string>

#include "object/teacher.h"
#include "object/student.h"
#include "object/course.h"
#include "object/job.h"
#include "object/list.h"
#include "data/teacherinterface.h"
#include "data/studentinterface.h"
#include "data/jobinterface.h"
using namespace std;

class TeachInterface {
public:
  TeachInterface() {
  }
  
  ~TeachInterface() {
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

  int addStudent(const Student&);
  int updateStudent(const Student&);
  int disableStudent(const string& user_id, bool available);
  Student getStudent(const string& user_id);
  StudentList getStudentList(int grade);
  StudentList getStudentList(int grade, int class_no);
  bool isStudent(const string& user_id);
  int addStudentToCourse(const string& user_id, int course_id);
  int deleteStudentFromCourse(const string& user_id, int course_id);

  int addJob(const Job&);
  int addJobToClass(int job_id, const Class& mclass);
  int addProblemToJob(int job_id, const vector<int>& problem_list);
  int addSet(const vector<int>& problem_list, int num);
  int addSetToJob(int job_id, int set_id);
  int disableJob(int job_id, bool available);
  JobList getJobList(int course_id);
  JobList getJobList(const string& student);
  Job getJob(int job_id);
  int deleteSetForJob(int job_id, int set_id);
  int updateJob(const Job&);
  int updateSet(const SetItem&);

  static TeachInterface& getInstance(){
    if(instance == NULL)
      instance = new TeachInterface;
    return *instance;
  }

  static void destroy(){
    if(instance)
      delete instance;
    instance = NULL;
  }
private:
  static TeachInterface * instance;
  
};


#endif
