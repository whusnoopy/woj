#ifndef _FLOOD_SERVER_DATA_CONNECTIONFACTORY_H__
#define _FLOOD_SERVER_DATA_CONNECTIONFACTORY_H__

#include <string>

#include "object/objectinc.h"
#include "data/result.h"
#include "data/mysqlconnection.h"
using namespace std;

class ConnectionFactory {
public:
  ConnectionFactory() {
  }
  
  ~ConnectionFactory() {
  }

  Connection* createConnection(const string& host,
                              const string& user,
                              const string& password,
                              const string& database);
  Connection* createConnection();

};

#endif
