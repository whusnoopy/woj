#ifndef _FLOOD_SERVER_DATA_FILE_H_
#define _FLOOD_SERVER_DATA_FILE_H_

#include <string>
using namespace std;

class File{
public:
  File(int file_id = 0, const string& path = "", int type = 0);
  
  int getFileId() const;
  string getPath() const;
  int getType() const;
  
  void setFileId(int file_id);
  void setPath(const string& path);
  void setType(int type);
private:
  int file_id_;
  string path_;
  int type_;
};

#endif /*FILE_H_*/
