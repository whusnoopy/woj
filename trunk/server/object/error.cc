#include "error.h"

Error::Error(int error_id,const string& content)
: error_id_(error_id), content_(content){

}
  
int Error::getErrorId() const {
  return error_id_;
}

string Error::getContent() const {
  return content_;
}
  
void Error::setErrorId(int error_id){
  error_id_ = error_id;
}

void Error::setContent(const string& content){
  content_ = content;
}
