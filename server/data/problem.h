#ifndef _FLOOD_SERVER_DATA_PROBLEM_H__
#define _FLOOD_SERVER_DATA_PROBLEM_H__

#include <string>
#include <iostream>
using namespace std;

#include "list.h"

class Problem{
public:
  Problem(int problem_id = 0) :  problem_id_(problem_id){
  }
  Problem(int problem_id,
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
          bool spj);
          
  int getProblemId() const;
  string getTitle() const;
  string getDescription() const;
  string getInput() const;
  string getOutput() const;
  string getSampleInput() const;
  string getSampleOutput() const;
  string getHint() const;
  string getSource() const;
  string getAddinTime() const;
  int getTimeLimit() const;
  int getCaseTimeLimit() const;
  int getMemoryLimit() const;
  int getAccepted() const;
  int getSubmit() const;
  int getSolvedUsers() const;
	int getSubmitUsers() const;
	int getStandardTimeLimit() const;
	int getStandardMemoryLimit() const;
	bool getAvailable() const;
	int getVersion() const;
	bool getSpj() const;
	void setProblemId(int problem_id);
  void setTitle(const string& title);
  void setDescription(const string& description);
  void setInput(const string& input);
  void setOutput(const string& output);
  void setSampleInput(const string& sample_input);
  void setSampleOutput(const string& sample_output);
  void setHint(const string& hint);
  void setSource(const string& source);
  void setAddinTime(const string& addin_time);
  void setTimeLimit(int time_limit);
  void setCaseTimeLimit(int case_time_limit);
  void setMemoryLimit(int memory_limit);
  void setAccepted(int accepted);
  void setSubmit(int submit); 	
  void setSolvedUsers(int solved_users);
	void setSubmitUsers(int submit_users);
	void setStandardTimeLimit(int standard_time_limit);
	void setStandardMemoryLimit(int standard_memory_limit);
	void setAvailable(bool available);
	void setVersion(int version);
	void setSpj(bool spj);
	
	ProblemListItem getProblemListItem() const;
	
private:
  int problem_id_;
  string title_;
  string description_;
  string input_;
  string output_;
  string sample_input_;
  string sample_output_;
  string hint_;
  string source_;
  string addin_time_;
  int time_limit_;
  int case_time_limit_;
  int memory_limit_;
  int accepted_;
  int submit_; 	
  int solved_users_;
	int submit_users_;
	int standard_time_limit_;
	int standard_memory_limit_;
	bool available_; 
	int version_;
	bool spj_;
	string infile_path_;
	string outfile_path_;
	
};

#endif 
