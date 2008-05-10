#include "teacherinterface.h"

TeacherInterface* TeacherInterface::instance = NULL;

int TeacherInterface::addStudent(const Teacher& teacher);

int TeacherInterface::updateStudent(const Teacher& teacher);

int TeacherInterface::disableStudent(const string& user_id);

int TeacherInterface::addControlClass(const string& user_id, const Class& mclass);

int TeacherInterface::deleteControlClass(const string& user_id, const Class& mclass);

int TeacherInterface::addControlClassList(const string& user_id, const ClassList& class_list);

