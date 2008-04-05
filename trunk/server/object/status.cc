#include "status.h"

Status::Status(int status_id,
               const string& user_id,
               int problem_id,
               int contest_id,
               int time,
               int memory,
               const string& submit_time,
               int result,
               int language,
               int code_id,
               int code_length,
               const string& submit_ip,
               int error_id)
: status_id_(status_id),
  user_id_(user_id),
  problem_id_(problem_id),
  contest_id_(contest_id),
  time_(time),
  memory_(memory),
  submit_time_(submit_time),
  result_(result),
  language_(language),
  code_id_(code_id),
  code_length_(code_length),
  submit_ip_(submit_ip),
  error_id_(error_id){
 }
  
         
int Status::getStatusId() const{
  return status_id_;
}

string Status::getUserId() const{
  return user_id_;
}

int Status::getProblemId() const{
  return problem_id_;
}

int Status::getContestId() const{
  return contest_id_;
}

int Status::getTime() const{
  return time_;
}

int Status::getMemory() const{
  return memory_;
}

string Status::getSubmitTime() const{
  return submit_time_;
}

int Status::getResult() const{
  return result_;
}

int Status::getLanguage() const{
  return language_;
}

int Status::getCodeId() const{
  return code_id_;
}

int Status::getCodeLength() const{
  return code_length_;
}

string Status::getSubmitIp() const{
  return submit_ip_;
}

int Status::getErrorId() const{
  return error_id_;
}

  
void Status::setStatusId(int status_id){
  status_id_ = status_id;
}

void Status::setUseId(const string& user_id){
  user_id_ = user_id;
}

void Status::setProblemId(int problem_id){
  problem_id_ = problem_id;
}

void Status::setContestId(int contest_id){
  contest_id_ = contest_id;
}

void Status::setTime(int time){
  time_ = time;
}

void Status::setMemory(int memory){
  memory_ = memory;
}

void Status::setSubmitTime(const string& submit_time){
  submit_time_ = submit_time;
}

void Status::setResult(int result){
  result_ = result;
}

void Status::setLanguage(int language){
  language_ = language;
}

void Status::setCodeId(int code_id){
  code_id_ = code_id;
}

void Status::setCodeLength(int code_length){
  code_length_ = code_length;
}

void Status::setSubmitIp(const string& submit_ip){
  submit_ip_ = submit_ip;
}

void Status::setErrorId(int error_id){
  error_id_ = error_id;
}
