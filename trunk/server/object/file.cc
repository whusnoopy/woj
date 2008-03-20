#include "file.h"

File::File(int file_id, const string& path, int type )
  :file_id_(file_id), path_(path), type_(type){
  }
  
int File::getFileId() const {
  return file_id_;
}
string File::getPath() const{
  return path_;
}

int File::getType() const {
  return type_;
}
 
void File::setFileId(int file_id){
	file_id_ = file_id;
}
void File::setPath(const string& path){
	path_ = path;
}
void File::setType(int type){
	type_ = type;
}
