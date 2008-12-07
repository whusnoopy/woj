#ifndef _FLOOD_SERVER_DATA_FILEINTERFACE_H__
#define _FLOOD_SERVER_DATA_FILEINTERFACE_H__

#include <string>
#include <iostream>
#include <sstream>

#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "server/object/objectinc.h"

using namespace std;

class FileInterface{
public:
  FileInterface(){
  }

  int addLink(const LinkList& link_list);
  int addFile(const string& filename, void* bufi, size_t filelength);
  FileData getFile(const string& filename);
  int updateFile(const string& filename, void* buf, size_t filelength);
  int updateLink(const LinkList& link_list);
  LinkList getLink();
  int updateNotice(const string& notice, const string& time);
  string getNotice();
  ssize_t getFileSize(const string& filename);
  
  static FileInterface& getInstance(){
    if(instance == NULL)
      instance = new FileInterface;
    return *instance;
  }

  static void destroy(){
    if(instance)
      delete instance;
    instance = NULL;
  }
private:
  static FileInterface * instance;
  
};


#endif
