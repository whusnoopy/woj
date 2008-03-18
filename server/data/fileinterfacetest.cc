#include <string>
#include <iostream>
#include <sstream>

#include "fileinterface.h"

/*
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <libxml/parser.h>

#include "filedata.h"
#include "configure.h"
#include "list.h"
#include "inc.h"
using namespace std;



void addDatabasetoConfigture(xmlNodePtr cur, Configure& configure){
	xmlChar * szKey;
	string host;
	string user;
	string password;
	string name;
	while (cur != NULL) {
	  if ((!xmlStrcmp(cur->name, (const xmlChar*) "host"))) {
	  	szKey = xmlNodeGetContent(cur);
	  	host = string((char *)szKey);
	  	xmlFree(szKey);
	  }
	  if ((!xmlStrcmp(cur->name, (const xmlChar*) "user"))) {
	  	szKey = xmlNodeGetContent(cur);
	  	user = string((char *)szKey);
	  	xmlFree(szKey);
	  }
	  if ((!xmlStrcmp(cur->name, (const xmlChar*) "password"))) {
	  	szKey = xmlNodeGetContent(cur);
	  	password = string((char *)szKey);
	  	xmlFree(szKey);
	  }
	  if ((!xmlStrcmp(cur->name, (const xmlChar*) "name"))) {
	  	szKey = xmlNodeGetContent(cur);
	  	name = string((char *)szKey);
	  	xmlFree(szKey);
	  }
	  cur = cur->next;
	}
	configure.setDatabase(host,user,password,name);
}
void addJudgeClienttoConfigture(xmlNodePtr cur, Configure& configure){
	xmlChar * szKey;
	while (cur != NULL) {
	  if ((!xmlStrcmp(cur->name, (const xmlChar*) "client"))) {
	  	xmlNodePtr node = cur->xmlChildrenNode;
	  	string ip;
	  	int port;
	  	while (node != NULL) {
	  	  if ((!xmlStrcmp(node->name, (const xmlChar*) "ip"))) {
	  	  	szKey = xmlNodeGetContent(node);
	  	    ip = string((char *)szKey);
	  	    xmlFree(szKey);
	  	  }
	  	  if ((!xmlStrcmp(node->name, (const xmlChar*) "port"))) {
	  	  	szKey = xmlNodeGetContent(node);
	  	    port = atoi(string((char *)szKey).c_str());
	  	    xmlFree(szKey);
	  	  }
	  	  node = node->next;
	  	}
	  }
	  cur = cur->next;
	}
}
void addLinktoConfigture(xmlNodePtr cur, Configure& configure){
	xmlChar * szKey;
	while (cur != NULL) {
	  if ((!xmlStrcmp(cur->name, (const xmlChar*) "path"))) {
	  	szKey = xmlNodeGetContent(cur);
	  	configure.setLinkPath(string((char *)szKey));
	  	xmlFree(szKey);
	  }
	  cur = cur->next;
	}
}
void addNoticetoConfigture(xmlNodePtr cur, Configure& configure){
	xmlChar * szKey;
	while (cur != NULL) {
	  if ((!xmlStrcmp(cur->name, (const xmlChar*) "path"))) {
	  	szKey = xmlNodeGetContent(cur);
	  	configure.setNoticePath(string((char *)szKey));
	  	xmlFree(szKey);
	  }
	  cur = cur->next;
	}
}

Configure getConfigure(){
	string configure_file = "./configure.xml";
	xmlDocPtr doc;
	xmlNodePtr cur;
	Configure configure;
	xmlChar* szKey;
  doc = xmlParseFile(configure_file.c_str());
  if (doc == NULL){
    cout << "open configure.xml error" << endl;
    return configure;
  }
  cur = xmlDocGetRootElement(doc);
  if (cur == NULL){
  	cout << "Empty configure." << endl;
  	xmlFreeDoc(doc);
  	return configure;
  }
  if (xmlStrcmp(cur->name, (const xmlChar *)"server")) {
    cout << "Wrong type document." << endl;
    xmlFreeDoc(doc);
    return configure;
  }
  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "database"))) {
      xmlNodePtr node = cur->xmlChildrenNode;
      addDatabasetoConfigture(node, configure);
    }
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "judgeclient"))) {
      xmlNodePtr node = cur->xmlChildrenNode;
      addJudgeClienttoConfigture(node, configure);
    }
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "link"))) {
      xmlNodePtr node = cur->xmlChildrenNode;
      addLinktoConfigture(node, configure);
    }
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "notice"))) {
      xmlNodePtr node = cur->xmlChildrenNode;
      addNoticetoConfigture(node, configure);
    }
    cur = cur->next;
  }
  xmlFreeDoc(doc);
  return configure;
}

int addLink(const LinkList& link_list){
	string link_file = getConfigure().getLinkPath();
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
}

int updateLink(const LinkList& link_list){
	string link_file = getConfigure().getLinkPath();
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
}

LinkList getLink(){
	string link_file = getConfigure().getLinkPath();
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
	close(linkfd);
	string str(buf);
	delete[] buf;
	
	istringstream in(str);
	while (in >> item.url >> item.site_name){
	  link_list.push_back(item);
	}
	return link_list;
}

int addFile(const string& filename, void * bufi, size_t filelength){
	int filefd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	ssize_t file_size = write(filefd, bufi, filelength);
	if ( file_size != filelength)
	  cout << "write failed." << endl;
	close(filefd);
}
FileData getFile(const string& filename){
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
int updateFile(const string& filename, void * buf, size_t filelength){
	int filefd = open(filename.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	ssize_t file_size = write(filefd, buf, filelength);
	if ( file_size != filelength)
	  cout << "write failed." << endl;
	close(filefd);
}



int updateNotice(const string& notice, const string& time){
	string notice_file = getConfigure().getNoticePath();
	int noticefd = open(notice_file.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
	string buf;
	buf = time + "\n" + notice;
	ssize_t file_size = write(noticefd, buf.c_str(), buf.length());
	if ( file_size != buf.length())
	  cout << "write failed." << endl;
	close(noticefd);
}
string getNotice(){
	string notice_file = getConfigure().getNoticePath();
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
*/

int main(){
	
	LinkListItem item;
	LinkList link_list;
	
	item.url = "http://www.macheng.com";
	item.site_name = "macheng";
	link_list.push_back(item);
	item.url = "http://www.apai.com";
	item.site_name = "apai";
	link_list.push_back(item);
	item.url = "http://www.lq.com";
	item.site_name = "lq";
	link_list.push_back(item);
	//cout << "ok" <<endl;
	FileInterface interface = FileInterface::getInstance();
	//cout << "ok" <<endl;
	interface.addLink(link_list);
	//cout << "ok" <<endl;
	link_list = interface.getLink();
	LinkList::iterator iter = link_list.begin();
	while (iter != link_list.end()){
	  cout << iter->url << "----" << iter->site_name << endl;
	  iter++;
	}
	
	
	string time = "2008-03-21s 10:00:00";
	string notice = "Eat \n adfafnasakdfak, ";
	//cout << time << endl;
	//cout << notice << endl;
	
	interface.updateNotice(notice, time);
	cout << interface.getNotice() << endl;
	
	/*
	Configure configure = getConfigure();
	cout << configure.getDatabaseHost() << endl;
	cout << configure.getDatabaseUser() << endl;
	cout << configure.getDatabasePassword() << endl;
	cout << configure.getDatabaseName() << endl;
	cout << configure.getLinkPath() << endl;
	cout << configure.getNoticePath() << endl;
	*/
	return 0;
}
