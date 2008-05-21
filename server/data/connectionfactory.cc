#include "connectionfactory.h"

#include <string>
using namespace std;

Connection* ConnectionFactory::createConnection(const string& host,
                                               const string& user,
                                               const string& password,
                                               const string& database){
  return new MysqlConnection(host, user, password, database);
}

Connection* ConnectionFactory::createConnection(){
	string host = Configure::getInstance().getDatabaseHost();
	string user = Configure::getInstance().getDatabaseUser();
	string password = Configure::getInstance().getDatabasePassword();
	string database = Configure::getInstance().getDatabaseName();
  return new MysqlConnection(host, user, password, database);
}


