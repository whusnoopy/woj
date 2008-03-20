#ifndef _FLOOD_SERVER_DATA_ERROR_H__
#define _FLOOD_SERVER_DATA_ERROR_H__

#include <string>
using namespace std;

class Error{
public:
  Error(int error_id = 0,const string& content = "");
  
  int getErrorId() const;
  string getContent() const;
  
  void setErrorId(int error_id);
  void setContent(const string& content);
 
private:
  int error_id_;
	string content_;
};

#endif /*ERROR_H_*/
