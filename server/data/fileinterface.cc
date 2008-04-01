#include "fileinterface.h"

#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

using namespace std;

FileInterface * FileInterface::instance = NULL;

int FileInterface::addLink(const LinkList& link_list){
	string link_file = Configure::getInstance().getLinkPath();
	//cout << "configureok" << endl;
	int linkfd = open(link_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	LinkList::const_iterator iter = link_list.begin();
	string buf;
	string line;
	while (iter != link_list.end()){
	  line = stringPrintf("%s %s\n", iter->url.c_str(), iter->site_name.c_str());
	  buf.append(line);
	  iter++;
	}
	//cout << buf << endl;
	ssize_t file_size = write(linkfd, buf.c_str(), buf.length());
	if ( file_size == -1)
	  cout << "write failed." << endl;
	close(linkfd);
  return 1;
}

int FileInterface::updateLink(const LinkList& link_list){
	string link_file = Configure::getInstance().getLinkPath();
	int linkfd = open(link_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	LinkList::const_iterator iter = link_list.begin();
	string buf;
	string line;
	while (iter != link_list.end()){
	  line = stringPrintf("%s %s\n", iter->url.c_str(), iter->site_name.c_str());
	  buf.append(line);
	  iter++;
	}
	//cout << buf << endl;
	ssize_t file_size = write(linkfd, buf.c_str(), buf.length());
	if ( file_size != buf.length())
	  cout << "write failed." << endl;
	close(linkfd);
  return 1;
}

LinkList FileInterface::getLink(){
	string link_file = Configure::getInstance().getLinkPath();
	struct stat file_stat;
	LinkListItem item;
	LinkList link_list;
	char* buf;
	if (stat(link_file.c_str(), &file_stat)){
	  cout << "Error in stat file " << endl;
	}
	ssize_t file_size = file_stat.st_size;
	buf = new char[file_size + 1];
	memset(buf, 0 ,file_size + 1);
	int linkfd = open(link_file.c_str(), O_RDONLY | O_CREAT , 0644);
	ssize_t size = read(linkfd, buf, file_size);
  if (size != file_size) {
    LOG(ERROR) << "Get Link failed";
  }
	close(linkfd);
	string str(buf);
	delete[] buf;
	
	istringstream in(str);
	while (in >> item.url >> item.site_name){
	  link_list.push_back(item);
	}
	return link_list;
}

int FileInterface::addFile(const string& filename, void * bufi, size_t filelength){
  string directory = filename.substr(0,filename.find_last_of("/"));
  if (access (directory.c_str(), F_OK) < 0) {
    if (errno == ENOENT) {
      if (mkdir(directory.c_str(), 0755) < 0) {
        LOG(SYS_ERROR) << "Cannot make the directory";
        return -1;
      }
      LOG(INFO) << "Create directory :" << directory;
    } else {
      LOG(SYS_ERROR) << "Cannot access the dir"; 
      return -1;
    }
  }
	int filefd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	ssize_t file_size = write(filefd, bufi, filelength);
	if ( file_size != filelength)
	  cout << "write failed." << endl;
	close(filefd);
  return 1;
}
FileData FileInterface::getFile(const string& filename){
	struct stat file_stat;
	FileData file_data;
	file_data.filename = filename;
	char* buf;
	if (stat(filename.c_str(), &file_stat)){
	  cout << "Error in stat file " << endl;
	}
	ssize_t file_size = file_stat.st_size;
	buf = new char[file_size];
	memset(buf, 0 ,file_size);
	int filefd = open(filename.c_str(), O_RDONLY , 0644);
	ssize_t size = read(filefd, buf, file_size);
	if (size != file_size)
	  cout << "Read error." << endl;
	close(filefd);
	file_data.buf = buf;
	return file_data;
}
int FileInterface::updateFile(const string& filename, void * buf, size_t filelength){
	string directory = filename.substr(0,filename.find_last_of("/"));
  if (access (directory.c_str(), F_OK) < 0) {
    if (errno == ENOENT) {
      if (mkdir(directory.c_str(), 0755) < 0) {
        LOG(SYS_ERROR) << "Cannot make the directory";
        return -1;
      }
      LOG(INFO) << "Create directory :" << directory;
    } else {
      LOG(SYS_ERROR) << "Cannot access the dir"; 
      return -1;
    }
  }
  int filefd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	ssize_t file_size = write(filefd, buf, filelength);
	if ( file_size != filelength)
	  cout << "write failed." << endl;
	close(filefd);
  return 1;
}

int FileInterface::updateNotice(const string& notice, const string& time){
	string notice_file = Configure::getInstance().getNoticePath();
	int noticefd = open(notice_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	string buf;
	buf = time + "\n" + notice;
	ssize_t file_size = write(noticefd, buf.c_str(), buf.length());
	if ( file_size != buf.length())
	  cout << "write failed." << endl;
	close(noticefd);
  return 1;
}
string FileInterface::getNotice(){
	string notice_file = Configure::getInstance().getNoticePath();
	struct stat file_stat;
	char* buf;
	if (stat(notice_file.c_str(), &file_stat)){
	  cout << "Error in stat file " << endl;
	}
	ssize_t file_size = file_stat.st_size;
	buf = new char[file_size + 1];
	memset(buf, 0 ,file_size + 1);
	int noticefd = open(notice_file.c_str(), O_RDONLY | O_CREAT , 0644);
	ssize_t size = read(noticefd, buf, file_size);
  if (size != file_size) {
    LOG(ERROR) << "getNotice failed";
  }
	close(noticefd);
	string str(buf);
	//cout << buf << endl;
	delete[] buf;
	string::size_type pos = str.find_first_of("\n");
	string time = str.substr(0, pos);
	string notice = str.substr(pos+1);
	string now_time = getLocalTimeAsString("%Y-%m-%d %H:%M:%S");
	if (now_time.compare(time) <= 0)
	  return notice;
	return string("");
}

ssize_t FileInterface::getFileSize(const string& filename) {
  struct stat file_stat;
	FileData file_data;
	file_data.filename = filename;
	if (stat(filename.c_str(), &file_stat)){
	  cout << "Error in stat file " << endl;
	}
	return file_stat.st_size;
}

