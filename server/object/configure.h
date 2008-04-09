#ifndef _FLOOL_SERVER_DATA_CONFIGURE_H__
#define _FLOOL_SERVER_DATA_CONFIGURE_H__

#include <string>
#include <vector>
#include <set>
#include <iostream>

#include <libxml/parser.h>
#include <pthread.h>
using namespace std;

class Configure{
public:
  Configure() {};
  Configure(const Configure& );

  void setDatabase(const string& host, 
                   const string& user, 
                   const string& password, 
                   const string& database);
  void setLinkPath(const string& path);
  void setNoticePath(const string& path);
  
  string getDatabaseHost() const ;
  string getDatabaseUser() const ;
  string getDatabasePassword() const ;
  string getDatabaseName() const ;
  string getLinkPath() const;
  string getNoticePath() const;
  int getJudgeControlMaxClient() const;
  int getJudgeControlPort() const;
  set<string> getJudgeControlIpTabs() const;
  int getNetWorkMaxClient() const;
  int getNetWorkPort() const;
  set<string> getNetWorkIpTabs() const;
  int getClientServerMaxClient() const;
  int getClientServerPort() const;

  static void init() {
    pthread_mutex_init(&lock, NULL);
  }
  
  static Configure& getInstance(){
    pthread_mutex_lock(&lock);
    pthread_mutex_unlock(&lock);
  	if (instance == NULL) {
  	  instance = createConfigure();
  	}
  	return *instance;
  }
  static void destroy(){
  	if (instance) {
  	  delete instance;
  	}
    instance = NULL;
    pthread_mutex_destroy(&lock);
  }

  static Configure reGet() {
    pthread_mutex_lock(&lock);
    if (instance) {
      delete instance;
    }
    instance = createConfigure();
    Configure conf(*instance);
    pthread_mutex_unlock(&lock);
    return conf;
  }
  
private:
  struct _DATABASE_CONFIGURE_{
    string host;
    string user;
    string password;
    string database;
  } database_configure;
  string linkpath;
  string noticepath;
  struct _JUDGECONTROL_CONFIGURE_{
    int max_client;
    int port;
    set<string> ip_tabs;
  } judgecontrol;
  struct _NETWORK_CONFIGURE_{
    int max_client;
    int port;
    set<string> ip_tabs;
  } network;
  struct _CLIENTSERVER_CONFIGURE_{
    int max_client;
    int port;
  } client_server;
  static Configure * instance;
  static pthread_mutex_t lock;
  static void addDatabasetoConfigture(xmlNodePtr cur, Configure& configure);
  static void addJudgeClienttoConfigture(xmlNodePtr cur, Configure& configure);
  static void addLinktoConfigture(xmlNodePtr cur, Configure& configure);
  static void addNoticetoConfigture(xmlNodePtr cur, Configure& configure);
  static void addNetWorktoConfigture(xmlNodePtr cur, Configure& configure);
  static void addClientServertoConfigture(xmlNodePtr cur, Configure& configure);
  static Configure* createConfigure();
};

#endif /*CONFIGURE_H_*/
