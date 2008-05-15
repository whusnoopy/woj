#include "teacherinterface.h"

TeacherInterface* TeacherInterface::instance = NULL;

int TeacherInterface::addTeacher(const Teacher& teacher) {
  Connection* connection = createConnection();  
  string query = "insert into teachers values(";
  query += "'" + changeSymbol(teacher.getUserId()) + "'," + 
           "'" + changeSymbol(teacher.getRealName()) + "'," + 
           "'" + changeSymbol(teacher.getAvailable() ? "Y" : "N") + "')";
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int TeacherInterface::updateTeacher(const Teacher& teacher) {
  Connection* connection = createConnection();  
  string query = "update teachers set ";
  query += "realname = '" + changeSymbol(teacher.getRealName()) + "' " +
           "where user_id = '" + changeSymbol(teacher.getUserId()) + "'";
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int TeacherInterface::disableTeacher(const string& user_id, bool available) {
  Connection* connection = createConnection();  
  string query = "update teachers set ";
  query += "available = '" + changeSymbol(available ? "Y" : "N") + "' " +
           "where user_id = '" + changeSymbol(user_id) + "'";
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int TeacherInterface::addControlClass(const string& user_id, const Class& mclass) {
  Connection* connection = createConnection();  
  string query = "insert into classestoteachers values(";
  query += "'" + changeSymbol(user_id) + "'," + 
           "'" + stringPrintf("%d", mclass.getGrade()) + "'," + 
           "'" + stringPrintf("%d", mclass.getClass()) + "')";
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int TeacherInterface::deleteControlClass(const string& user_id, const Class& mclass) {
  Connection* connection = createConnection();  
  string query = "delete from classestoteachers where";
  query += "user_id = '" + changeSymbol(user_id) + "' " + 
           "and grade = '" + stringPrintf("%d", mclass.getGrade()) + "' " + 
           "and class = '" + stringPrintf("%d", mclass.getClass()) + "' ";
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int TeacherInterface::addControlClassList(const string& user_id, const ClassList& class_list);

