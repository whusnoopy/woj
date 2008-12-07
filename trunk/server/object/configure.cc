// Copyright 2008 Flood Team of Wuhan Univ.

#include <pthread.h>

#include <libxml/parser.h>

#include "base/flags.h"
#include "base/logging.h"
#include "base/util.h"

#include "server/object/inc.h"

#include "server/object/configure.h"

DEFINE_FLAGS(string, configure_path, "absolutely path of configure.xml");

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

void Configure::setJudgeControl(const set<string>& ip_table,
                                const int& port,
                                const int& max_client) {
  judgecontrol.ip_tabs = ip_table;
  judgecontrol.port = port;
  judgecontrol.max_client = max_client;
}

void Configure::setNetwork(const set<string>& ip_table,
                           const int& port,
                           const int& max_client) {
  network.ip_tabs = ip_table;
  network.port = port;
  network.max_client = max_client;
}

void Configure::setClientServer(const int& port,
                                const int& max_client) {
  client_server.port = port;
  client_server.max_client = max_client;
}

void Configure::output() const{
  set<string> ips = getJudgeControlIpTabs();
  set<string>::iterator iter = ips.begin();
  while (iter != ips.end()) {
    cout << "judge_control_ip:" << *iter;
    iter++;
  }
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

static void addDatabasetoConfigture(xmlNodePtr cur, Configure& configure){
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

static void addJudgeClienttoConfigture(xmlNodePtr cur, Configure& configure){
	xmlChar * szKey;
  set<string> ip_table;
  int port = 0;
  int max_client = 0;
	while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "ip"))) {
      szKey = xmlNodeGetContent(cur);
      string ip = string((char *)szKey);
      ip_table.insert(ip);
      xmlFree(szKey);
    }
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "port"))) {
      szKey = xmlNodeGetContent(cur);
      port = atoi(string((char *)szKey).c_str());
      xmlFree(szKey);
    }
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "maxclient"))) {
      szKey = xmlNodeGetContent(cur);
      max_client = atoi(string((char *)szKey).c_str());
      xmlFree(szKey);
    }
	  cur = cur->next;
	}
  configure.setJudgeControl(ip_table, port, max_client);
}

static void addNetWorktoConfigture(xmlNodePtr cur, Configure& configure){
	xmlChar * szKey;
  set<string> ip_table;
  int port = 0;
  int max_client = 0;
	while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "ip"))) {
      szKey = xmlNodeGetContent(cur);
      string ip = string((char *)szKey);
      ip_table.insert(ip);
      xmlFree(szKey);
    }
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "port"))) {
      szKey = xmlNodeGetContent(cur);
      port = atoi(string((char *)szKey).c_str());
      xmlFree(szKey);
    }
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "maxclient"))) {
      szKey = xmlNodeGetContent(cur);
      max_client = atoi(string((char *)szKey).c_str());
      xmlFree(szKey);
    } 
	  cur = cur->next;
	}
  configure.setNetwork(ip_table, port, max_client);
}

static void addClientServertoConfigture(xmlNodePtr cur, Configure& configure){
	xmlChar * szKey;
  int port = 0;
  int max_client = 0;
	while (cur != NULL) {
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "port"))) {
      szKey = xmlNodeGetContent(cur);
      port = atoi(string((char *)szKey).c_str());
      xmlFree(szKey);
    }
    if ((!xmlStrcmp(cur->name, (const xmlChar*) "maxclient"))) {
      szKey = xmlNodeGetContent(cur);
      max_client = atoi(string((char *)szKey).c_str());
      xmlFree(szKey);
    } 
	  cur = cur->next;
	}
  configure.setClientServer(port, max_client);
}

static void addLinktoConfigture(xmlNodePtr cur, Configure& configure){
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

static void addNoticetoConfigture(xmlNodePtr cur, Configure& configure){
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
	Configure* configure = new Configure;
  
  xmlDocPtr doc = xmlParseFile(FLAGS_configure_path.c_str());
  if (doc == NULL) {
    LOG(ERROR) << "Can't open " << FLAGS_configure_path << " as configure.xml";
    return configure;
  }

  // Check the configure.xml is valid or not
  xmlNodePtr cur = xmlDocGetRootElement(doc);
  if (cur == NULL){
  	LOG(ERROR) << "File " << FLAGS_configure_path << " is empty";
  	xmlFreeDoc(doc);
  	return configure;
  }
  if (xmlStrcmp(cur->name, (const xmlChar *)"server")) {
    LOG(ERROR) << "File format of " << FLAGS_configure_path << " unacceptable";
    xmlFreeDoc(doc);
    return configure;
  }

  // Configure from configure.xml
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
      addClientServertoConfigture(node, *configure);
    }

    cur = cur->next;
  }

  xmlFreeDoc(doc);
  return configure;
}

