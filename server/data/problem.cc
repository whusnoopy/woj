#include "problem.h"

Problem::Problem(int problem_id,
        const string& title,
        const string& description,
        const string& input,
        const string& output,
        const string& sample_input,
        const string& sample_output,
        const string& hint,
        const string& source,
        const string& addin_time,
        int time_limit,
        int case_time_limit,
        int memory_limit,
        int accepted,
        int submit,
        int solved_users,
        int submit_users,
        int standard_time_limit,
        int standard_memory_limit,
        bool available,
        int version,
        bool spj)
 :  problem_id_(problem_id),
    title_(title),
    description_(description),
    input_(input),
    output_(output),
    sample_input_(sample_input),
    sample_output_(sample_output),
    hint_(hint),
    source_(source),
    addin_time_(addin_time),
    time_limit_(time_limit),
    case_time_limit_(case_time_limit),
    memory_limit_(memory_limit),
    accepted_(accepted),
    submit_(submit),
    solved_users_(solved_users),
    submit_users_(submit_users),
    standard_time_limit_(standard_time_limit),
    standard_memory_limit_(standard_memory_limit),
    available_(available),
    version_(version),
    spj_(spj){
}
          
int Problem::getProblemId() const{
	return problem_id_;
}

string Problem::getTitle() const{
	return title_;
}

string Problem::getDescription() const{
  return description_;
}

string Problem::getInput() const{
  return input_;
}

string Problem::getOutput() const{
  return output_;
}

string Problem::getSampleInput() const{
  return sample_input_;
}

string Problem::getSampleOutput() const{
  return sample_output_;
}

string Problem::getHint() const{
  return hint_;
}

string Problem::getSource() const{
  return source_;
}

string Problem::getAddinTime() const{
  return addin_time_;
}

int Problem::getTimeLimit() const{
  return time_limit_;
}

int Problem::getCaseTimeLimit() const{
  return case_time_limit_;
}

int Problem::getMemoryLimit() const{
  return memory_limit_;
}

int Problem::getAccepted() const{
  return accepted_;
}

int Problem::getSubmit() const{
  return submit_;
}
	
int Problem::getSolvedUsers() const{
  return solved_users_;
}

int Problem::getSubmitUsers() const{
  return submit_users_;
}

int Problem::getStandardTimeLimit() const{
  return standard_time_limit_;
}

int Problem::getStandardMemoryLimit() const{
  return standard_memory_limit_;
}

bool Problem::getAvailable() const{
  return available_;
}

int Problem::getVersion() const{
  return version_;
}

bool Problem::getSpj() const{
  return spj_;
}

void Problem::setProblemId(int problem_id){
  problem_id_ = problem_id;
}

void Problem::setTitle(const string& title){
  title_ = title;
}

void Problem::setDescription(const string& description){
  description_ = description;
}

void Problem::setInput(const string& input){
  input_ = input;
}

void Problem::setOutput(const string& output){
  output_ = output;
}

void Problem::setSampleInput(const string& sample_input){
  sample_input_ = sample_input;
}

void Problem::setSampleOutput(const string& sample_output){
  sample_output_ = sample_output;
}

void Problem::setHint(const string& hint){
  hint_ = hint;
}

void Problem::setSource(const string& source){
  source_ = source;
}

void Problem::setAddinTime(const string& addin_time){
  addin_time_ = addin_time;
}

void Problem::setTimeLimit(int time_limit){
  time_limit_ = time_limit;
}

void Problem::setCaseTimeLimit(int case_time_limit){
  case_time_limit_ = case_time_limit;
}

void Problem::setMemoryLimit(int memory_limit){
  memory_limit_ = memory_limit;
}

void Problem::setAccepted(int accepted){
  accepted_ = accepted;
}

void Problem::setSubmit(int submit){
  submit_ = submit;
}
	
void Problem::setSolvedUsers(int solved_users){
  solved_users_ = solved_users;
}

void Problem::setSubmitUsers(int submit_users){
  submit_users_ = submit_users;
}

void Problem::setStandardTimeLimit(int standard_time_limit){
  standard_time_limit_ = standard_time_limit;
}

void Problem::setStandardMemoryLimit(int standard_memory_limit){
  standard_memory_limit_ = standard_memory_limit;
}

void Problem::setAvailable(bool available){
  available_ = available;
}


void Problem::setVersion(int version){
  version_ = version;
}
	
void Problem::setSpj(bool spj){
	spj_ = spj;
}	

ProblemListItem Problem::getProblemListItem() const{
  ProblemListItem item;
  item.accepted = accepted_;
  item.problem_id = problem_id_;
  item.submit = submit_;
  item.title = title_;
  return item;
}

