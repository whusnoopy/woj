#include "studentinterface.h"

StudentInterface* StudentInterface::instance = NULL;

int StudentInterface::addStudent(const Student& student) {
  Connection* connection = createConnection();  
  string query = "insert into students values(";
  query += "'" + changeSymbol(student.getUserId()) + "'," + 
           "'" + changeSymbol(student.getStudentId()) + "'," +
           "'" + changeSymbol(student.getRealName()) + "'," + 
       //    "'" + changeSymbol(student.getClass.getCollege()) + "'," +
           "'" + stringPrintf("%d", student.getClass.getGrade()) + "'," +
           "'" + stringPrintf("%d", student.getClass.getClass()) + "'," +
           "'" + changeSymbol(student.getAvailable() ? "Y" : "N") + "')";
  connection->connect();
  int ret = connection->executeUpdate(query);
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
           "grade = '" + stringPrintf("%d", student.getClass.getGrade()) + "'," +
           "class = '" + stringPrintf("%d", student.getClass.getClass()) + "' " +
           "where user_id = '" + changeSymbol(student.getUserId()) + "'"; 
  connection->connect();
  int ret = connection->executeUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int StudentInterface::disableStudent(const string& user_id, bool available) {
  Connection* connection = createConnection();  
  string query = "update students set ";
  query += "available = '" + available ? "Y" : "N" + "' " + 
           "where user_id = '" + changeSymbol(student.getUserId()) + "'"; 
  connection->connect();
  int ret = connection->executeUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

Student StudentInterface::getStudent(const string& user_id) {
  Connection* connection = createConnection();  
  string query = "select * from students where user_id = '";
  query += changeSymbol(user_id) + "'"; 
  Student student;
  student.setUserId("NULL");
  Class mclass;
  connection->connect();
  Result result_set = connection->executeQuery(query);
  if (reault_set.next()) {
    student.setUserId(result_set.getString("user_id"));
    student.setStudentId(result_set.getString("student_id"));
    student.setRealName(result_set.getString("realname"));
 //   mclass.setCollege(result_set.getString("college"));
    mclass.setGrade(result_set.getInt("grade"));
    mclass.setClass(result_set.getInt("class"));
    student.setClass(mclass);
    student.setAvailable(result_set.getString("available") == "Y");
  }
  connection->close();
  delete connection;
  return student;
}

