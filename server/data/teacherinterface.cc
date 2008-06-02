#include "teacherinterface.h"

#include "object/status.h"
#include <set>
using namespace std;

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
  string query = "insert into courses(description, available, user_id) values(";
  query += "'" + changeSymbol(description) + "', 'Y', "; 
  query += "'" + changeSymbol(user_id) + "')";
  connection->connect();
  connection->excuteUpdate(query);
  query = "select LAST_INSERT_ID() as course_id";
  int course_id = 0;
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    course_id = result_set.getInt("course_id");
  }
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

Teacher TeacherInterface::getTeacher(const string& user_id) {
  Connection* connection = createConnection(); 
  Teacher teacher;
  string query = "select * from teachers where user_id = '";
  query += changeSymbol(user_id) + "'";
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    teacher.setUserId(result_set.getString("user_id"));
    teacher.setRealName(result_set.getString("realname"));
    teacher.setAvailable((result_set.getString("available") == "Y"));
  }
  result_set.close();
  connection->close();
  delete connection;
  return teacher;
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

int TeacherInterface::setStatusResult(int status_id, int result) {
  Connection* connection = createConnection(); 
  string query = "update statuses set ";
  query += stringPrintf(" result = '%d' ", result);
  query += stringPrintf(" where status_id = '%d'", status_id);
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

StatusList TeacherInterface::getStatusListForTeacher(int job_id, int page_id) {
  Connection* connection = createConnection(); 
  StatusList status_list;
  Status item;
  set<int> problem_set;
  string query;
  connection->connect();
  query = "select problem_id from problemstojobs where job_id = " + stringPrintf("'%d'", job_id);
  LOG(DEBUG) << query;
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()){
    problem_set.insert(result_set.getInt("problem_id"));
  }
  result_set.close();
  query = "select problem_id from problemstosets where set_id in (";
  query += "select set_id from setstojobs where job_id = " + stringPrintf("'%d')", job_id);
  LOG(DEBUG) << query;
  result_set = connection->excuteQuery(query);
  while (result_set.next()){
    problem_set.insert(result_set.getInt("problem_id"));
  }
  result_set.close();
  query = "select * from statuses "; 
  query += stringPrintf(" where problem_id in ('-600'");
  set<int>::iterator problem_set_iter = problem_set.begin();
  while (problem_set_iter != problem_set.end()) {
    query += stringPrintf(", '%d'", *problem_set_iter);
    problem_set_iter++;
  }
  query += ") and type = 'F'";
  query += stringPrintf(" limit %d, 25", page_id * 25);
  LOG(DEBUG) << query;
  result_set= connection->excuteQuery(query);
  while(result_set.next()){
  	item.setStatusId(result_set.getInt("status_id"));
	  item.setUserId(result_set.getString("user_id"));
	  item.setProblemId(result_set.getInt("problem_id"));
	  item.setContestId(result_set.getInt("contest_id"));
	  item.setTime(result_set.getInt("time"));
	  item.setMemory(result_set.getInt("memory"));
	  item.setSubmitTime(result_set.getString("submit_time"));
	  item.setResult(result_set.getInt("result"));
	  item.setLanguage(result_set.getInt("language"));
	  item.setCodeId(result_set.getInt("code_id"));
	  item.setCodeLength(result_set.getInt("code_length"));
	  item.setSubmitIp(result_set.getString("submit_ip"));
	  item.setErrorId(result_set.getInt("error_id"));
  	status_list.push_back(item);
  }
  result_set.close();
  connection->close();
  delete connection;
  return status_list;
}

//int TeacherInterface::addControlClassList(const string& user_id, const ClassList& class_list);

