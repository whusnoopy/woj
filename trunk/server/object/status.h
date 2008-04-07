#ifndef _FLOOD_SERVER_DATA_STATUS_H__
#define _FLOOD_SERVER_DATA_STATUS_H__

#include <string>
using namespace std;

class Status{
public:
  Status(int status_id = 0) : status_id_(status_id){
  }
  Status(int status_id,
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
	       int error_id);
	       
	int getStatusId() const;
	string getUserId() const;
	int getProblemId() const;
	int getContestId() const;
	int getTime() const;
	int getMemory() const;
	string getSubmitTime() const;
	int getResult() const;
	int getLanguage() const;
	int getCodeId() const;
	int getCodeLength() const;
	string getSubmitIp() const;
	int getErrorId() const;
	
	void setStatusId(int status_id);
	void setUseId(const string& user_id);
	void setProblemId(int problem_id);
	void setContestId(int contest_id);
	void setTime(int time);
	void setMemory(int memory);
	void setSubmitTime(const string& submit_time);
	void setResult(int result);
	void setLanguage(int language);
	void setCodeId(int code_id);
	void setCodeLength(int code_length);
	void setSubmitIp(const string& submit_ip);
	void setErrorId(int error_id);
	
	
  
private:
  int status_id_;
	string user_id_;
	int problem_id_;
	int contest_id_;
	int time_;
	int memory_;
	string submit_time_;
	int result_;
	int language_;
	int code_id_;
	int code_length_;
	string submit_ip_;
	int error_id_;
};

bool operator<(const Status& a, const Status& b);

#endif /*STATUS_H_*/
