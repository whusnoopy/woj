#include <string>
#include <iostream>

#include "data/result.h"
#include "data/connection.h"
#include "data/mysqlconnection.h"
#include "object/objectinc.h"
#include "base/judge_result.h"
#include "base/logging.h"
#include "base/flags.h"
#include "base/util.h"

using namespace std;

DEFINE_FLAGS(int, type, "is day, week or month? [1/7/30]");

Connection* createConnection(const string& host, 
                            const string& user, 
                            const string& password,
                            const string& name) {
  return new MysqlConnection(host, user, password, name);
}

string day_count;
string update_set;

void parseType(int type) {
  switch (type) {
    case 1:
      day_count = "1";
      update_set = "day_user";
      break;
    case 7:
      day_count = "7";
      update_set = "week_user";
      break;
    default :
      day_count = "30";
      update_set = "month_user";
      break;
  }
}

void update() {
  Connection* connection = createConnection("127.0.0.1", "root", "123456", "flood");
  connection->connect();
  string query = "select user_id from statuses where type = 'N' and result = '" + stringPrintf("%d'", ACCEPTED);
  query += " and date_sub(current_timestamp(), interval " + day_count + " day) <= submit_time group by user_id order by "
           "count(distinct(problem_id)) desc";
  string user_id= "NULL";
  LOG(DEBUG) << query ;
  Result result_set = connection->excuteQuery(query);
  if (result_set.next()) {
    user_id = result_set.getString("user_id");
  }
  result_set.close();
  if (user_id == "NULL") { 
    LOG(INFO) << "There is no one submit during " + day_count + " days.";
    return ;
  }
  query = "update most_diligen_player set " + update_set + " = '" + changeSymbol(user_id) + "'";
  connection->excuteUpdate(query);
  LOG(INFO) << "New " + update_set + " is " + user_id; 
  connection->close();
  delete connection;
}

int main(int argc, char ** argv) { 
  if (parseFlags(argc,argv)) {
    LOG(SYS_ERROR) << "Cannot parse flags!";
    return -1;
  }
  
  parseType(FLAGS_type);
  update();
  return 0;
}


