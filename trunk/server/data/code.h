#ifndef _FLOOD_SERVER_DATA_CODE_H__
#define _FLOOD_SERVER_DATA_CODE_H__

#include <string>
using namespace std;

class Code{
public:
  Code(int code_id = 0, bool share = false, const string& code_content = "");
  
  int getCodeId() const;
  bool getShare() const;
  string getCodeContent() const;
  
  void setCodeId(int code_id);
  void setShare(bool share);
  void setCodeContent(const string& code_content);
  
private:
  int code_id_;
	bool share_;
	string code_content_;
};

#endif /*CODE_H_*/
