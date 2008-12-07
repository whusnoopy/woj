// Copyright 2008 Flood Team of Wuhan Univ.
// Author : ooeyusea@gmail.com (Qian, LIU)
// Modified : yewen@mail.whu.edu.cn (Wen, YE)

#include "base/logging.h"
#include "base/util.h"

#include "server/data/studentinterface.h"

StudentInterface* StudentInterface::instance = NULL;

int StudentInterface::addStudent(const Student& student) {
  Connection* connection = createConnection();  
  string query = "insert into students values(";
  query += "'" + changeSymbol(student.getUserId()) + "'," + 
           "'" + changeSymbol(student.getStudentId()) + "'," +
           "'" + changeSymbol(student.getRealName()) + "'," + 
       //    "'" + changeSymbol(student.getClass.getCollege()) + "'," +
           "'" + stringPrintf("%d", student.getClass().getGrade()) + "'," +
           "'" + stringPrintf("%d", student.getClass().getClass()) + "'," +
           "'" + changeSymbol(student.getAvailable() ? "Y" : "N") + "')";
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int StudentInterface::updateStudent(const Student& student) {
  Connection* connection = createConnection();  
  string query = "update students set ";
  query += "student_id = '" + changeSymbol(student.getStudentId()) + "'," +
           "realname = '" + changeSymbol(student.getRealName()) + "'," + 
      //     "college = '" + changeSymbol(student.getClass.getCollege()) + "'," +
           "grade = '" + stringPrintf("%d", student.getClass().getGrade()) + "'," +
           "class = '" + stringPrintf("%d", student.getClass().getClass()) + "' " +
           "where user_id = '" + changeSymbol(student.getUserId()) + "'"; 
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int StudentInterface::disableStudent(const string& user_id, bool available) {
  Connection* connection = createConnection();  
  string query = "update students set ";
  query += "available = '" + changeSymbol(available ? "Y" : "N") + "' where user_id = '" + 
           changeSymbol(user_id) + "'"; 
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

StudentList StudentInterface::getStudentList(int course_id) {
  Connection* connection = createConnection();
  string query = "select * from students where user_id in ("
                 "select user_id from studentstocourse  where"
                 " course_id = '";
  query += stringPrintf("%d')", course_id);
  LOG(DEBUG) << query; 
  StudentList student_list;
  StudentItem item;
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    item.user_id = result_set.getString("user_id");
    item.realname = result_set.getString("realname");
    item.student_id = result_set.getString("student_id");
    item.available = (result_set.getString("available") == "Y");
    item.mclass.setGrade(result_set.getInt("grade"));
    item.mclass.setClass(result_set.getInt("class"));
    item.score = 0;
    student_list.push_back(item);
  }
  connection->close();
  delete connection;
  return student_list;
}

StudentList StudentInterface::getStudentList(int grade, int class_no) {
  Connection* connection = createConnection();
  string query = "select * from students where grade = '";
  query += stringPrintf("%d'", grade);
  query += " and class = '" + stringPrintf("%d'", class_no);
  StudentList student_list;
  StudentItem item;
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    item.user_id = result_set.getString("user_id");
    item.realname = result_set.getString("realname");
    item.student_id = result_set.getString("student_id");
    item.available = (result_set.getString("available") == "Y");
    item.mclass.setGrade(result_set.getInt("grade"));
    item.mclass.setClass(result_set.getInt("class"));
    item.score = 0;
    student_list.push_back(item);
  }
  connection->close();
  delete connection;
  return student_list;
}

Student StudentInterface::getStudent(const string& user_id) {
  Connection* connection = createConnection();  
  string query = "select * from students where user_id = '";
  query += changeSymbol(user_id) + "'"; 
  Student student;
  student.setUserId("NULL");
  Class mclass;
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    student.setUserId(result_set.getString("user_id"));
    student.setStudentId(result_set.getString("student_id"));
    student.setRealName(result_set.getString("realname"));
 //   mclass.setCollege(result_set.getString("college"));
    mclass.setGrade(result_set.getInt("grade"));
    mclass.setClass(result_set.getInt("class"));
    student.setClass(mclass);
    student.setAvailable(result_set.getString("available") == "Y");
  }
  result_set.close();
  connection->close();
  delete connection;
  return student;
}

bool StudentInterface::isStudent(const string& user_id) {
  Connection* connection = createConnection();  
  string query = "select * from students where user_id = '";
  query += changeSymbol(user_id) + "'"; 
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  bool ret = result_set.next();
  result_set.close();
  connection->close();
  delete connection;
  return ret;
}

int StudentInterface::addStudentToCourse(const string& user_id, int course_id) {
  Connection* connection = createConnection();  
  string query = "insert into studentstocourse(user_id, course_id) values (";
  query += "'" + changeSymbol(user_id) + "', ";
  query += stringPrintf("'%d')", course_id);
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int StudentInterface::deleteStudentFromCourse(const string& user_id, int course_id) {
  Connection* connection = createConnection();  
  string query = "delete from studentstocourse where ";
  query += "user_id = '" + changeSymbol(user_id) + "' and";
  query += " course_id = '" + stringPrintf("%d'", course_id);
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}


