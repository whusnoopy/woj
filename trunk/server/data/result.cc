#include "result.h"

#include "object/inc.h"
using namespace std;

Result::Result(MYSQL_RES* result){
	m_result = result;
	int num_fields = mysql_num_fields(result);
	MYSQL_FIELD * field = mysql_fetch_fields(result);
	for(int i = 0; i < num_fields; i++){
	  m_name_map[field[i].name]=i;
	}
}

Result::~Result(){  	
}

bool Result::next(){
  m_row = mysql_fetch_row(m_result);
  if(m_row)
		return true;
	else
		return false;
}
/*
bool Result::previous(){
  m_row =  
}*/
int Result::getRowNum(){
  return mysql_num_rows(m_result);
}

string Result::getString(const string& name){
  if(m_name_map.count(name) == 0){
		cout << "No such field:" << name << endl;
		string str;
		return str;
	}else{
	  int l = m_name_map[name];
	  if (m_row[l] != NULL){
		  string str(m_row[l]);
		  return str;
	  }else {
	  	return string("");
	  }
	}
}

int Result::getInt(const string& name){
  if(m_name_map.count(name) == 0){
		cout << "No such field."<< name << endl;
		return 0;
	}else{
	  int l = m_name_map[name],ret;
		string str = "0";
    if (m_row[l] != NULL) {
      str = string(m_row[l]);
    }
	  ret = atoi(str.c_str());
		return ret;
	} 
}
int Result::getInt(int n) {
  string str(m_row[n]);
  return atoi(str.c_str());
}

void Result::close(){
  if (m_result != NULL) {
    LOG(DEBUG) << "m_result is empty?" << (m_result == NULL);
    mysql_free_result(m_result);
	  m_result = NULL;
  }
  LOG(DEBUG) << "m_result is empty?" << (m_result == NULL);
  LOG(DEBUG) << "free the result";
}

