#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "mysql.h"
#include "woj.h"

int main()
{
  MYSQL *connection,mysql;
  MYSQL_RES *result;
  MYSQL_ROW row;
  string query_cmd;
  //initial mysql
  mysql_init(&mysql);
  connection = mysql_real_connect(&mysql,HOST,USER,PASSWORD,DATABASE,0,NULL,0);
  query_cmd = "select user_id from solution where result = 0 and date_sub(current_timestamp(),interval 7 day) <= in_date   group by user_id order by count(distinct(problem_id)) desc";
  mysql_query(connection,query_cmd.c_str());
  result=mysql_store_result(connection);
  row=mysql_fetch_row(result);
  if(row != NULL){
    query_cmd = "update statusinfo set weekdeligent=\'"+(string)row[0]+"\'";
    // cout<<query_cmd<<endl;
    mysql_query(connection,query_cmd.c_str());
  }
  return 0;
}    
   
  
  
