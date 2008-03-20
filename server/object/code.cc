#include "code.h"

Code::Code(int code_id, bool share, const string& code_content)
: code_id_(code_id), share_(share), code_content_(code_content){

}
  
int Code::getCodeId() const{
	return code_id_;
}

bool Code::getShare() const{
  return share_;
}

string Code::getCodeContent() const{
  return code_content_;
}
  
void Code::setCodeId(int code_id){
  code_id_ = code_id;
}

void Code::setShare(bool share){
  share_ = share;
}

void Code::setCodeContent(const string& code_content){
  code_content_ = code_content;
}

