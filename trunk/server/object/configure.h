// Copyright 2008 Flood Team of Wuhan Univ.

#ifndef _FLOOL_SERVER_DATA_CONFIGURE_H__
#define _FLOOL_SERVER_DATA_CONFIGURE_H__

#include <string>
#include <vector>
#include <set>
#include <iostream>

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
  void setJudgeControl(const set<string>& ip_table,
                       const int& port,
                       const int& max_client);
  void setNetwork(const set<string>& ip_table,
                  const int& port,
                  const int& max_client);
  void setClientServer(const int& port,
                       const int& max_client);
  void setLinkPath(const string& path);
  void setNoticePath(const string& path);
  
  void output() const;
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
  	if (instance == NULL) {
  	  instance = createConfigure();
  	}
    pthread_mutex_unlock(&lock);
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
  string linkpath;
  string noticepath;

  static Configure * instance;
  static pthread_mutex_t lock;
  
  static Configure* createConfigure();
};

#endif // end of _FLOOD_SERVER_OBJECT_CONFIGURE_H__

