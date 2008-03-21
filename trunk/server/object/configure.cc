#include "configure.h"

Configure* Configure::instance = NULL;

void Configure::setDatabase(const string& host, 
                            const string& user, 
                            const string& password, 
                            const string& database) {
  database_configure.host = host;
  database_configure.user = user;
  database_configure.password = password;
  database_configure.database = database;// = {host, user, password, database};
}

string Configure::getDatabaseHost() const {
	return database_configure.host;
}

string Configure::getDatabaseUser() const {
	return database_configure.user;
}

string Configure::getDatabasePassword() const {
	return database_configure.password;
}

string Configure::getDatabaseName() const {
	return database_configure.database;
}

void Configure::setLinkPath(const string& path) {
	linkpath = path;
}
void Configure::setNoticePath(const string& path) {
	noticepath = path;
}
string Configure::getLinkPath() const {
  return linkpath;
}
string Configure::getNoticePath() const {
  return noticepath;
}

void Configure::addDatabasetoConfigture(xmlNodePtr cur, Configure& configure){
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
void Configure::addJudgeClienttoConfigture(xmlNodePtr cur, Configure& configure){
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
void Configure::addLinktoConfigture(xmlNodePtr cur, Configure& configure){
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
void Configure::addNoticetoConfigture(xmlNodePtr cur, Configure& configure){
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

Configure* Configure::createConfigure(){
	string configure_file = "./configure.xml";
	xmlDocPtr doc;
	xmlNodePtr cur;
	Configure* configure = new Configure;
	//xmlChar* szKey;
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
      addDatabasetoConfigture(node, *configure);
    }
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "judgeclient"))) {
      xmlNodePtr node = cur->xmlChildrenNode;
      addJudgeClienttoConfigture(node, *configure);
    }
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "link"))) {
      xmlNodePtr node = cur->xmlChildrenNode;
      addLinktoConfigture(node, *configure);
    }
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "notice"))) {
      xmlNodePtr node = cur->xmlChildrenNode;
      addNoticetoConfigture(node, *configure);
    }
    cur = cur->next;
  }
  xmlFreeDoc(doc);
  return configure;
}


