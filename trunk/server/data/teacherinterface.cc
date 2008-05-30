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

int TeacherInterface::addControlClass(const string& user_id, const string& description) {
  Connection* connection = createConnection();  
  string query = "insert into courses(description, user_id) values(";
  query += "'" + changeSymbol(description) + "',"; 
  query += "'" + changeSymbol(user_id) + "')";
  int course_id = 0;
  connection->connect();
  connection->excuteUpdate(query);
  query = "select last_insert_id() as course_id";
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) 
    course_id = result_set.getInt("course_id");
  result_set.close();
  connection->close();
  delete connection;
  return course_id;
}

int TeacherInterface::deleteControlClass(const string& user_id, int course_id) {
  Connection* connection = createConnection();  
  string query = "update courses set available = 'N' where";
  query += "user_id = '" + changeSymbol(user_id) + "' " + 
           "and course_id = '" + stringPrintf("%d", course_id) + "' ";
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

TeacherList TeacherInterface::getTeacherList() {
  Connection* connection = createConnection(); 
  TeacherList teacher_list;
  string query = "select * from teachers";
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    string user_id = result_set.getString("user_id");
    string realname = result_set.getString("realname");
    bool available = (result_set.getString("available") == "Y");
    teacher_list.push_back(Teacher(user_id, realname, available));
  }
  result_set.close();
  connection->close();
  delete connection;
  return teacher_list;
}

bool TeacherInterface::isTeacher(const string& user_id) {
  Connection* connection = createConnection(); 
  TeacherList teacher_list;
  string query = "select * from teachers where user_id = '";
  query += changeSymbol(user_id) + "'";
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  bool ret = result_set.next();
  result_set.close();
  connection->close();
  delete connection;
  return ret;
}

CourseList TeacherInterface::getCourseList(const string& user_id) {
  Connection* connection = createConnection(); 
  CourseList course_list;
  CourseItem item;
  string query = "select * from courses where ";
  query += "user_id = '" + changeSymbol(user_id) + "'";
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    item.course_id = result_set.getInt("course_id");
    item.description = result_set.getString("description");
    item.teacher = result_set.getString("user_id");
    item.available = result_set.getString("available") == "Y";
    course_list.push_back(item);
  }
  result_set.close();
  connection->close();
  delete connection;
  return course_list;
}

Course TeacherInterface::getCourse(int course_id) {
  Connection* connection = createConnection(); 
  Course course;
  string query = "select * from courses where ";
  query += "course_id = '" + stringPrintf("%d", course_id) + "' and available = 'Y'";
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    int course_id = result_set.getInt("course_id");
    string description = result_set.getString("description");
    string teacher = result_set.getString("user_id");
    bool available = result_set.getString("available") == "Y";
    course = Course(course_id, description, teacher, available);
  }
  result_set.close();
  connection->close();
  delete connection;
  return course;
}

//int TeacherInterface::addControlClassList(const string& user_id, const ClassList& class_list);

