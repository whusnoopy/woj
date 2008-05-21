#include "teachinterface.h"

TeachInterface* TeachInterface::instance = NULL;

int TeachInterface::addTeacher(const Teacher& teacher) {
  return TeacherInterface::getInstance().addTeacher(teacher);
}

int TeachInterface::updateTeacher(const Teacher& teacher) {
  return TeacherInterface::getInstance().updateTeacher(teacher);
}

int TeachInterface::disableTeacher(const string& user_id, bool available) {
  return TeacherInterface::getInstance().disableTeacher(user_id, available);
}

int TeachInterface::addControlClass(const string& user_id, const Class& mclass) {
  return TeacherInterface::getInstance().addControlClass(user_id, mclass);
}

int TeachInterface::deleteControlClass(const string& user_id, const Class& mclass) {
  return TeacherInterface::getInstance().deleteControlClass(user_id, mclass);
}

int TeachInterface::getTeacherList() {
  return TeacherInterface::getInstance().getTeacherList();
}

int TeachInterface::isTeacher(const string& user_id) {
  return TeacherInterface::getInstance().isTeacher(user_id);
}

int TeachInterface::addStudent(const Student& student) {
  return StudentInterface::getInstance().addStudent(student);
}

int TeachInterface::updateStudent(const Student& student) {
  return StudentInterface::getInstance().updateStudent(student);
}

int TeachInterface::disableStudent(const string& user_id, bool available) {
  return StudentInterface::getInstance().disableStudent(user_id, available);
}

StudentList TeachInterface::getStudentList(int grade) {
  return StudentInterface::getInstance().getStudentList(grade);
}

StudentList TeachInterface::getStudentList(int grade, int class_no) {
  return StudentInterface::getInstance().getStudentList(grade, class_no);
}

Student TeachInterface::getStudent(const string& user_id) {
  return StudentInterface::getInstance().getStudent(user_id);
}

Student TeachInterface::isStudent(const string& user_id) {
  return StudentInterface::getInstance().isStudent(user_id);
}


int TeachInterface::addJob(const Job& job) {
  return JobInterface::getInstance().addJob(job);
}

int TeachInterface::addJobToClass(int job_id, const Class& mclass) {
  return JobInterface::getInstance().addJobToClass(job_id, mclass);
}

int TeachInterface::addProblemToJob(int job_id, const vector<int>& problem_list) {
  return JobInterface::getInstance().addProblemToJob(job_id, problem_list);
}

int TeachInterface::addSet(const vector<int>& problem_list, int num) {
  return JobInterface::getInstance().addSet(problem_list, num);
}

int TeachInterface::addSetToJob(int job_id, int set_id) {
  return JobInterface::getInstance().addSetToJob(job_id, set_id);
}

int TeachInterface::disableJob(int job_id, bool available) {
  return JobInterface::getInstance().disableJob(job_id, available);
}

JobList TeachInterface::getJobList(const string& teacher) {
  return JobInterface::getInstance().getJobList(teacher);
}

Job TeachInterface::getJob(int job_id) {
  return JobInterface::getInstance().getJob(job_id);
}

int TeachInterface::deleteSetForJob(int job_id, int set_id) {
  return JobInterface::getInstance().deleteSetForJob(job_id, set_id);
}

int TeachInterface::updateJob(const Job& job) {
  return JobInterface::getInstance().updateJob(job);
}

int TeachInterface::updateSet(const SetItem& set_item) {
  return JobInterface::getInstance().updateSet(set_item);
}

