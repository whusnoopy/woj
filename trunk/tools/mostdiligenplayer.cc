#include <string>
#include <iostream>

#include "base/flags.h"
#include "base/judge_result.h"
#include "base/logging.h"
#include "base/util.h"

#include "server/data/result.h"
#include "server/data/connection.h"
#include "server/data/mysqlconnection.h"
#include "server/object/objectinc.h"

using namespace std;

DEFINE_FLAGS(int, type, "is day, week or month? [1/7/30]");

const string db_address = "localhost";
const string db_user = "root";
const string db_password = "123456";
const string db_name = "flood";

Connection* connection = NULL;

Connection* createConnection(const string& host, 
                             const string& user, 
                             const string& password,
                             const string& name) {
  return new MysqlConnection(host, user, password, name);
}

string day_count = "1";
string update_set = "day_user";

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

string getMostDeligenPlayer() {
  string query = "select user_id from statuses where type = 'N' and result = '" +
                 stringPrintf("%d'", ACCEPTED) + " and date_sub(current_timestamp(), "
                 "interval " + day_count + " day) <= submit_time group by user_id "
                 "order by count(distinct(problem_id)) desc";
  string user_id= "NULL";

  LOG(DEBUG) << query ;
  Result result_set = connection->excuteQuery(query);
  if (result_set.next())
    user_id = result_set.getString("user_id");

  result_set.close();

  if (user_id == "NULL")
    LOG(INFO) << "There is no submit in the past " + day_count + " days.";
  else
    LOG(INFO) << "The most diligen player in the past " + day_count + " is " + user_id;

  return user_id;
}

int main(int argc, char* argv[]) { 
  if (parseFlags(argc, argv)) {
    LOG(SYS_ERROR) << "Cannot parse flags!";
    return -1;
  }

  parseType(FLAGS_type);

  connection = createConnection(db_address, db_user, db_password, db_name);
  connection->connect();
  
  string user_id = getMostDeligenPlayer();

  string query = "update most_diligen_player set " + update_set + " = '" +
                 changeSymbol(user_id) + "'";
  connection->excuteUpdate(query);

  connection->close();
  delete connection;

  return 0;
}

