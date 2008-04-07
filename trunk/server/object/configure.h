#ifndef _FLOOL_SERVER_DATA_CONFIGURE_H__
#define _FLOOL_SERVER_DATA_CONFIGURE_H__

#include <string>
#include <vector>
#include <iostream>

#include <libxml/parser.h>
using namespace std;

class Configure{
public:
  Configure() {};

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
  vector<string> getJudgeControlIpTabs() const;
  int getNetWorkMaxClient() const;
  int getNetWorkPort() const;
  vector<string> getNetWorkIpTabs() const;
  
  static Configure& getInstance(){
  	if (instance == NULL) {
  	  instance = createConfigure();
  	}
  	return *instance;
  }
  static void destroy(){
  	if (instance) {
  	  delete instance;
  	}
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
    vector<string> ip_tabs;
  } judgecontrol;
  struct _NETWORK_CONFIGURE_{
    int max_client;
    int port;
    vector<string> ip_tabs;
  } network;
  static Configure * instance;
  static void addDatabasetoConfigture(xmlNodePtr cur, Configure& configure);
  static void addJudgeClienttoConfigture(xmlNodePtr cur, Configure& configure);
  static void addLinktoConfigture(xmlNodePtr cur, Configure& configure);
  static void addNoticetoConfigture(xmlNodePtr cur, Configure& configure);
  static void addNetWorktoConfigture(xmlNodePtr cur, Configure& configure);
  static Configure* createConfigure();
};

#endif /*CONFIGURE_H_*/
