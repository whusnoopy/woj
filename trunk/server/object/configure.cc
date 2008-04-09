#include "configure.h"

#include "inc.h"
#include <pthread.h>

Configure* Configure::instance = NULL;
pthread_mutex_t Configure::lock;

Configure::Configure(const Configure& other) {
  database_configure = other.database_configure;
  linkpath = other.linkpath;
  noticepath = other.noticepath;
  judgecontrol = other.judgecontrol;
  network = other.network;
  client_server = other.client_server;
}

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

int Configure::getJudgeControlMaxClient() const {
  return judgecontrol.max_client;
}

int Configure::getJudgeControlPort() const {
  return judgecontrol.port;
}

set<string> Configure::getJudgeControlIpTabs() const {
  return judgecontrol.ip_tabs;
}

int Configure::getNetWorkMaxClient() const {
  return network.max_client;
}

int Configure::getNetWorkPort() const {
  return network.port;
}

int Configure::getClientServerMaxClient() const {
  return client_server.max_client;
}

int Configure::getClientServerPort() const {
  return client_server.port;
}

set<string> Configure::getNetWorkIpTabs() const {
  return network.ip_tabs;
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
    xmlNodePtr node = cur;
    string ip;
    int port;
    int max_client;
    if ((!xmlStrcmp(node->name, (const xmlChar*) "ip"))) {
      szKey = xmlNodeGetContent(node);
      ip = string((char *)szKey);
      configure.judgecontrol.ip_tabs.insert(ip);
      xmlFree(szKey);
    }
    if ((!xmlStrcmp(node->name, (const xmlChar*) "port"))) {
      szKey = xmlNodeGetContent(node);
      port = atoi(string((char *)szKey).c_str());
      configure.judgecontrol.port = port;
      xmlFree(szKey);
    }
    if ((!xmlStrcmp(node->name, (const xmlChar*) "maxclient"))) {
      szKey = xmlNodeGetContent(node);
      max_client = atoi(string((char *)szKey).c_str());
      configure.judgecontrol.max_client = max_client;
      xmlFree(szKey);
    }
	  cur = cur->next;
	}
}

void Configure::addNetWorktoConfigture(xmlNodePtr cur, Configure& configure){
	xmlChar * szKey;
	while (cur != NULL) {
    xmlNodePtr node = cur;
    string ip;
    int port;
    int max_client;
    if ((!xmlStrcmp(node->name, (const xmlChar*) "ip"))) {
      szKey = xmlNodeGetContent(node);
      ip = string((char *)szKey);
      configure.network.ip_tabs.insert(ip);
      xmlFree(szKey);
    }
    if ((!xmlStrcmp(node->name, (const xmlChar*) "port"))) {
      szKey = xmlNodeGetContent(node);
      port = atoi(string((char *)szKey).c_str());
      configure.network.port = port;
      xmlFree(szKey);
    }
    if ((!xmlStrcmp(node->name, (const xmlChar*) "maxclient"))) {
      szKey = xmlNodeGetContent(node);
      max_client = atoi(string((char *)szKey).c_str());
      configure.network.max_client = max_client;
      xmlFree(szKey);
    } 
	  cur = cur->next;
	}
}

void Configure::addClientServertoConfigture(xmlNodePtr cur, Configure& configure){
	xmlChar * szKey;
	while (cur != NULL) {
    xmlNodePtr node = cur;
    string ip;
    int port;
    int max_client;
    if ((!xmlStrcmp(node->name, (const xmlChar*) "port"))) {
      szKey = xmlNodeGetContent(node);
      port = atoi(string((char *)szKey).c_str());
      configure.client_server.port = port;
      xmlFree(szKey);
    }
    if ((!xmlStrcmp(node->name, (const xmlChar*) "maxclient"))) {
      szKey = xmlNodeGetContent(node);
      max_client = atoi(string((char *)szKey).c_str());
      configure.client_server.max_client = max_client;
      xmlFree(szKey);
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
    LOG(ERROR) << "open configure.xml error";
    return configure;
  }
  cur = xmlDocGetRootElement(doc);
  if (cur == NULL){
  	LOG(ERROR) << "Empty configure.";
  	xmlFreeDoc(doc);
  	return configure;
  }
  if (xmlStrcmp(cur->name, (const xmlChar *)"server")) {
    LOG(ERROR) << "Wrong type document.";
    xmlFreeDoc(doc);
    return configure;
  }
  cur = cur->xmlChildrenNode;
  while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "database"))) {
      xmlNodePtr node = cur->xmlChildrenNode;
      addDatabasetoConfigture(node, *configure);
    }
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "judgecontrol"))) {
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
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "network"))) {
      xmlNodePtr node = cur->xmlChildrenNode;
      addNetWorktoConfigture(node, *configure);
    }
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "client"))) {
      xmlNodePtr node = cur->xmlChildrenNode;
      addNetWorktoConfigture(node, *configure);
    }


    cur = cur->next;
  }
  xmlFreeDoc(doc);
  return configure;
}


