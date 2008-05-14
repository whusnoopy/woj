#include "job.h"

Job::Job(const string& description) 
  : description_(description){
} 
  
vector<int> Job::getProblemList() const {
  return problem_list_;
}

vecotr<SetItem> Job::getSetList() const {
  return set_list_;
}

int Job::getJobId() const{
  return job_id_;
}

string Job::getDescription() const {
  return description_;
}

string Job::getPublishTime() const {
  return publish_time_;
}

string Job::getTeacher() const {
  return user_id_;
}

int Job::getYear() const {
  return year_;
}

char Job::getTerm() const {
  return term_;
}

bool Job::getAvailable() const {
  return available_;
}


  
void Job::setProblemList(const vector<int>& problem_list) {
  problem_list_ = problem_list;
}

void Job::setSetList(const vector<SetItem>& set_list) {
  set_list_ = set_list;
}

void Job::setJobId(int job_id) {
  job_id_ = job_id;
}

void Job::setDescription(const string& description) {
  description_ = description;
}

void Job::setPublishTime(const string& publish_time) {
  publish_time_ = publish_time;
}

void Job::setTeacher(const string& user_id) {
  user_id_ = user_id;
}

void Job::setYear(int year){
  year_ = year;
}

void Job::setTerm(char term){
  term_ = term;
}

void Job::setAvailable(bool available){
  available_ = available;
}


