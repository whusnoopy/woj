// Copyright 2008 Flood Team of Wuhan Univ.
// Author : ooeyusea@gmail.com (Qian, LIU)
// Modified : yewen@mail.whu.edu.cn (Wen, YE)

#include "base/logging.h"
#include "base/util.h"

#include "server/data/jobinterface.h"

JobInterface* JobInterface::instance = NULL;

int JobInterface::addJob(const Job& job) {
  Connection* connection = createConnection();  
  string query = "insert into jobs(description, publish_time, course_id, year, term, available) values (";
  query += "'" + changeSymbol(job.getDescription()) + "'," +
           "'" + changeSymbol(job.getPublishTime()) + "'," +
           "'" + stringPrintf("%d", job.getCourseId()) + "'," +
           "'" + stringPrintf("%d", job.getYear()) + "'," +
           "'" + stringPrintf("%c", job.getTerm()) + "'," +
           "'" + changeSymbol(job.getAvailable() ? "Y" : "N") + "')";
  connection->connect();
  connection->excuteUpdate(query);
  query = "select last_insert_id() as job_id";
  Result result_set = connection->excuteQuery(query);
  int job_id = 0;
  if (result_set.next()) {
    job_id = result_set.getInt("job_id");
  }
  result_set.close();
  connection->close();
  delete connection;
  return job_id;
}

int JobInterface::addJobToClass(int job_id, const Class& mclass) {
  Connection* connection = createConnection();  
  string query = "insert into jobstoclass(job_id, grade, class) values (";
  query += "'" + stringPrintf("%d", job_id) + "'," +
           "'" + stringPrintf("%d", mclass.getGrade()) + "'," +
           "'" + stringPrintf("%d", mclass.getClass()) + "')";
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int JobInterface::addProblemToJob(int job_id, const vector<int>& problem_list) {
  Connection* connection = createConnection();  
  connection->connect();
  string delete_query = "delete from problemstojobs where job_id = '" + stringPrintf("%d'", job_id);
  connection->excuteUpdate(delete_query);
  vector<int>::const_iterator problem_iter = problem_list.begin();
  int ret = 0;
  while (problem_iter != problem_list.end()) {
    string query = "insert into problemstojobs(job_id, problem_id) values (";
    query += "'" + stringPrintf("%d", job_id) + "'," +
             "'" + stringPrintf("%d", *problem_iter) + "')";
    LOG(DEBUG) << query;
    ret = connection->excuteUpdate(query);
    problem_iter++;
  }  
  connection->close();
  delete connection;
  return ret;
}
int JobInterface::addSet(const vector<int>& problem_list, int num) {
  Connection* connection = createConnection();  
  connection->connect();
  string query = "insert into sets(number) values(";
  query += "'" + stringPrintf("%d", num) + "')";
  LOG(DEBUG) << query;
  int ret = connection->excuteUpdate(query);
  query = "select last_insert_id() as set_id";
  Result result_set = connection->excuteQuery(query);
  int set_id = 0;
  if (result_set.next()) {
    set_id = result_set.getInt("set_id");
  }
  result_set.close();
  LOG(DEBUG) << "here is ok";
  vector<int>::const_iterator problem_iter = problem_list.begin();
  while (problem_iter != problem_list.end()) {
    query = "insert into problemstosets(set_id, problem_id) values (";
    query += "'" + stringPrintf("%d", set_id) + "'," +
             "'" + stringPrintf("%d", *problem_iter) + "')";
    LOG(DEBUG) << query;
    ret = connection->excuteUpdate(query);
    problem_iter++;
  }
  connection->close();
  delete connection;
  return set_id;
}
int JobInterface::addSetToJob(int job_id, int set_id) {
  Connection* connection = createConnection();  
  connection->connect();
  string query = "insert into setstojobs(job_id, set_id) values (";
  query += "'" + stringPrintf("%d", job_id) + "'," +
           "'" + stringPrintf("%d", set_id) + "')"; 
  LOG(DEBUG) << query;
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int JobInterface::disableJob(int job_id, bool available) {
  Connection* connection = createConnection();  
  string query = "update jobs set ";
  query += "available = '" + changeSymbol(available ? "Y" : "N") + "' " +
           "where job_id = '" + stringPrintf("%d", job_id) + "'";
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

JobList JobInterface::getJobList(int course_id) {
  Connection* connection = createConnection();  
  string query = "select * from jobs where course_id = '";
  query += stringPrintf("%d", course_id) + "'";
  LOG(DEBUG) << query;
  JobItem item;
  JobList job_list;
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    item.job_id = result_set.getInt("job_id");
    item.description = result_set.getString("description");
    item.publish_time = result_set.getString("publish_time");
    item.course_id = result_set.getInt("course_id");
    item.year = result_set.getInt("year");
    item.term = *(result_set.getString("term").c_str());
    item.available = (result_set.getString("available") == "Y");
    job_list.push_back(item);
  }
  connection->close();
  delete connection;
  return job_list;
}

JobList JobInterface::getJobList(const string& student) {
  Connection* connection = createConnection();  
  string query = "select * from jobs where course_id in (";
  query += " select course_id from studentstocourse where user_id = '";
  query += changeSymbol(student) + "')";
  LOG(DEBUG) << query;
  JobItem item;
  JobList job_list;
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    item.job_id = result_set.getInt("job_id");
    item.description = result_set.getString("description");
    item.publish_time = result_set.getString("publish_time");
    item.course_id = result_set.getInt("course_id");
    item.year = result_set.getInt("year");
    item.term = *(result_set.getString("term").c_str());
    item.available = (result_set.getString("available") == "Y");
    job_list.push_back(item);
  }
  connection->close();
  delete connection;
  return job_list;
}

Job JobInterface::getJob(int job_id) {
  Connection* connection = createConnection();  
  string query = "select * from jobs where job_id = '";
  query += stringPrintf("%d", job_id) + "'";
  LOG(DEBUG) << query;
  Job job; 
  job.setJobId(0);
  connection->connect();
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    job.setJobId(result_set.getInt("job_id"));
    job.setDescription(result_set.getString("description"));
    job.setPublishTime(result_set.getString("publish_time"));
    job.setCourseId(result_set.getInt("course_id"));
    job.setYear(result_set.getInt("year"));
    job.setTerm(*(result_set.getString("term").c_str()));
    job.setAvailable((result_set.getString("available") == "Y"));
  }
  result_set.close();
  query = "select * from problemstojobs where job_id = '";
  query += stringPrintf("%d", job_id) + "'";
  LOG(DEBUG) << query;
  result_set = connection->excuteQuery(query);
  vector<int> must_do_problem_list;
  while (result_set.next()) {
    must_do_problem_list.push_back(result_set.getInt("problem_id"));
  }
  result_set.close();
  job.setProblemList(must_do_problem_list);
  vector<SetItem> set_list;
  SetItem item;
  query = "select sets.set_id, number from sets, setstojobs where ";
  query += "sets.set_id = setstojobs.set_id and job_id = '";
  query += stringPrintf("%d", job_id) + "'";
  LOG(DEBUG) << query;
  result_set = connection->excuteQuery(query);
  while (result_set.next()) {
    item.set_id = result_set.getInt("set_id");
    item.number = result_set.getInt("number");
    set_list.push_back(item);
  }
  result_set.close();
  vector<SetItem>::iterator set_iter = set_list.begin();
  while (set_iter != set_list.end()) {
    int set_id = set_iter->set_id;
    query = "select * from problemstosets where set_id = '";
    query += stringPrintf("%d'", set_id);
    LOG(DEBUG) << query;
    vector<int> should_do_problem_list;
    result_set = connection->excuteQuery(query);
    while (result_set.next()) {
      should_do_problem_list.push_back(result_set.getInt("problem_id"));
    }
    set_iter->problem_list = should_do_problem_list;
    result_set.close();
    set_iter++;
  }
  job.setSetList(set_list);
  connection->close();
  delete connection;
  return job;
}

int JobInterface::deleteSetForJob(int job_id, int set_id) {
  Connection* connection = createConnection();  
  connection->connect();
  string delete_query = "delete from setstojobs where job_id = '" + 
                        stringPrintf("%d' and ", job_id) + 
                        stringPrintf("set_id = '%d'", set_id);
  int ret = connection->excuteUpdate(delete_query);
  connection->close();
  delete connection;
  return ret;
}

int JobInterface::updateJob(const Job& job) {
  Connection* connection = createConnection();  
  string query = "update jobs set";
  query += "description = '" + changeSymbol(job.getDescription()) + "'," +
           "publish_time = '" + changeSymbol(job.getPublishTime()) + "'," + 
           "course_id = '" + stringPrintf("%d", job.getCourseId()) + "'," +
           "year = '" + stringPrintf("%d", job.getYear()) + "'," +
           "term = '" + stringPrintf("%c", job.getTerm()) + "' " +
           " where job_id = '" + stringPrintf("%d'", job.getJobId());
  connection->connect();
  int ret = connection->excuteUpdate(query);
  connection->close();
  delete connection;
  return ret;
}

int JobInterface::updateSet(const SetItem& set_item) {
  Connection* connection = createConnection();  
  connection->connect();
  string query = "update sets set ";
  query += "number = '" + stringPrintf("%d", set_item.number) + "' where ";
  query += "set_id = '" + stringPrintf("%d'", set_item.set_id);
  int ret = connection->excuteUpdate(query);
  string delete_query = "delete from problemstosets where set_id = '";
  delete_query += stringPrintf("%d'", set_item.set_id); 
  ret = connection->excuteUpdate(delete_query);
  vector<int>::const_iterator problem_iter = set_item.problem_list.begin();
  while (problem_iter != set_item.problem_list.end()) {
    string query = "insert into problemstosets(set_id, problem_id) values (";
    query += "'" + stringPrintf("%d", set_item.set_id) + "'," +
             "'" + stringPrintf("%d", *problem_iter) + "')"; 
    ret = connection->excuteUpdate(query);
  }
  connection->close();
  delete connection;
  return ret;
}
