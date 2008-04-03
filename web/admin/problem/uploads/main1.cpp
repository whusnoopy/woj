# include "mysql.h"
# include "woj.h"

//knuthocean begin
string getMyTimestr(int);
string getWrongSubmitstr(int);
int myDiff(string,string);
string getResultStr(int);
string getLanguageStr(int);
//knuthocean end

string language_map[10]= {".c",".cpp","java",".pas",".pp"};
string status_map[20] =	{
			"ACCEPT",
			"PRESENTATION_ERROR",
			"TIME_LIMIT",
			"MEMORY_LIMIT",
			"WRONG_ANSWER",
			"OUTPUT_LIMIT",
			"COMPILE_ERROR",
			"RUNTIME_ERROR_SIGSEGV",
			"RUNTIME_ERROR_SIGFPE",
			"RUNTIME_ERROR_SIGBUS",
			"RUNTIME_ERROR_SIGABRT",
			"RUNTIME_ERROR_JAVA",
			"RUNTIME_ERROR_PACAL",
			"RESTRICTED_FUNCTION",
			"ERROR_INT"
			};
string source_output_path;

int generate_code(const string solution_id,MYSQL *connection,int language){
	string query_cmd;
	MYSQL_RES *result;
	MYSQL_ROW row;
	FILE *fp;
	
	int state;
	char buf[2048];
	/////////////////////////////////////////////////////////////////////////////////
	// 把sql中的source_code中的source转化为程序文件
	/////////////////////////////////////////////////////////////////////////////////
	query_cmd="SELECT source FROM source_code WHERE solution_id="+solution_id;
	state=mysql_query(connection,query_cmd.c_str());
	if (state!=0) {
		//printf(mysql_error(connection));
		return 1;
	}
	if(opendir(WORK_PATH.c_str())==NULL) 
		mkdir(WORK_PATH.c_str(),S_IRWXU);
	/* must call mysql_store_result(  ) before you can issueany other query calls */
    	result=mysql_store_result(connection);
	if ( (row=mysql_fetch_row(result))!=NULL ) {
		if(language==2){
			if(mkdir((WORK_PATH+"/"+solution_id).c_str(),S_IRWXU)<0) return 1;
			source_output_path=WORK_PATH+"/"+solution_id+"/Main.java";
		}
		else {
			source_output_path=WORK_PATH+"/"+solution_id+language_map[language];
		}
		if( (fp=fopen(source_output_path.c_str(),"w+"))==NULL) return 1;
		fputs(row[0],fp);
		fclose(fp);
	}
	mysql_free_result(result);
	return 0;
}
int get_info(	const string solution_id,
		string &problem_id,
		string &input_path,
		int &language,
		int &time_limit,
		int &case_time_limit,
		int &memory_limit
	    )
{
	//printf("%s\n",solution_id.c_str());
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL *connection, mysql;
	int state,state2;
	string query_cmd;
	FILE *fp;
	// connect to the MySQL database at localhost 
	if(mysql_init(&mysql)==NULL) return 1;

	connection = mysql_real_connect(&mysql, HOST, USER, PASSWORD, DATABASE, 0, NULL, 0);
	
	// check for a connection error 
	if(connection==NULL) {	
		return 1;
	}
	query_cmd="SELECT problem_id,solution_id,language FROM solution WHERE solution_id="+solution_id;
	state=mysql_query(connection,query_cmd.c_str());
	if (state!=0) {
		return 1;
	}
	// must call mysql_store_result(  ) before you can issueany other query calls 
    	result=mysql_store_result(connection);
    	// process each row in the result set 
    	if ( (row=mysql_fetch_row(result))!=NULL ) {
		//printf("%s\n",row[2]);
		sscanf(row[2],"%d",&language);
		//printf("languagae = %d\n",language);
		if(generate_code(solution_id,connection,language)) return 1;
		query_cmd="SELECT problem_id,input_path,output_path,time_limit,case_time_limit,memory_limit FROM problem WHERE        problem_id="+(string)row[0];	
		//printf("%s\n",query_cmd.c_str());
		state=mysql_query( connection, query_cmd.c_str() );	
		if (state != 0) {
			//printf(mysql_error(connection));
			return 1;
		}
		result=mysql_store_result(connection);
    		//printf("Rows: %d\n", mysql_num_rows(result));
		if( (row = mysql_fetch_row(result))!=NULL ) {
			problem_id 	= (string)row[0];
			input_path	= (string)row[1];
			time_limit	= atoi(row[3]);
			case_time_limit	= atoi(row[4]);
			memory_limit	= atoi(row[5]);
 		}
 	}
	else {
		return 1;
	}	
 	mysql_free_result(result);
 	mysql_close(connection);
	return 0;
}
char cmd[1<<15];
void update_user(run_info user_info,string solution_id){

	MYSQL_RES *result,*result2;
	MYSQL_ROW row,row2;
	MYSQL *connection,mysql;
	int state;
	string query_cmd;
	char time[50];
	char memory[50];
	char uresult[50];
	FILE *fp;
	//把int转为char *类型
	sprintf(time,"%d",user_info.run_time);
	sprintf(memory,"%d",user_info.run_memory);
	sprintf(uresult,"%d",user_info.status);
	//初试化mysql
	mysql_init(&mysql);
	connection = mysql_real_connect(&mysql, HOST, USER, PASSWORD, DATABASE, 0, NULL, 0);
	//查找problem_id
	query_cmd="SELECT problem_id,user_id FROM solution WHERE solution_id="+solution_id;
	//cout<<query_cmd.c_str()<<endl;
	mysql_query(connection,query_cmd.c_str());
	result=mysql_store_result(connection);
	row=mysql_fetch_row(result);//row[0]就是problem了,row[1]就是user。

        //test if the user has ac that problem before
	query_cmd="select count(*) from solution where user_id = \'"+(string)row[1]+"\' and problem_id="+(string)row[0]+" and result=0";
    mysql_query(connection,query_cmd.c_str());
	result2=mysql_store_result(connection);
    row2 = mysql_fetch_row(result2);

	query_cmd="UPDATE solution SET time="+(string)time+",memory="+(string)memory+",result="+(string)uresult+" WHERE solution_id="+solution_id;
	mysql_query(connection,query_cmd.c_str());
	query_cmd="UPDATE problem SET submit = submit + 1 WHERE problem_id="+(string)row[0];
	mysql_query(connection,query_cmd.c_str());
	query_cmd="UPDATE user SET submit = submit + 1 WHERE user_id= \'"+(string)row[1]+"\'";
	mysql_query(connection,query_cmd.c_str());
	
	if(user_info.status==0){ //AC++
		query_cmd="UPDATE problem SET accepted = accepted + 1 WHERE problem_id="+(string)row[0];
		mysql_query(connection,query_cmd.c_str());
                if((string)row2[0] == "0"){
			query_cmd="UPDATE user SET solved = solved + 1 WHERE user_id= \'"+(string)row[1]+"\'";
			mysql_query(connection,query_cmd.c_str());
		}
	}
	else if(user_info.CE_info!=""){ //CE
		//query_cmd="INSERT into compileinfo values("+solution_id+",'"+user_info.CE_info+"')";
 		//printf("%s\n",user_info.CE_info.c_str());
		sprintf(cmd,"INSERT into compileinfo values(%s,'%s')",solution_id.c_str(),user_info.CE_info.c_str());
		mysql_query(connection,cmd);
	}
	else if(user_info.JAVA_RE_info!=""){ //JAVA_RE
		query_cmd="INSERT into javareinfo values("+solution_id+",\'"+user_info.JAVA_RE_info+"\')";
		//cout<<query_cmd<<endl;
		mysql_query(connection,query_cmd.c_str());
	}
	else if(user_info.PAS_RE_info!=""){ //PASCAL_RE
		query_cmd="INSERT into pascalreinfo values("+solution_id+",\'"+user_info.PAS_RE_info+"\')";
		mysql_query(connection,query_cmd.c_str());
	}
	//knuthocean begin
   	 int acceptnum,addpenaltytime;
    	acceptnum = (user_info.status == 0) ? 1:0; //whether the problem AC
    	
 	 //  query_cmd = "select user_id,contest_id,problem_id,in_date from solution"+" where solution_id="+solution_id;
	query_cmd="select user_id,contest_id,problem_id,in_date,result,language from solution where solution_id="+solution_id;
  	mysql_query(connection,query_cmd.c_str());
  	result2 = mysql_store_result(connection);
  	row2 = mysql_fetch_row(result2);
   	string myUser_id,myContest_id,myProblem_id,myIn_date,myUserNum,myNick,myResult,myLanguage;
  	myUser_id = (string)row2[0];
   	myContest_id = (string)row2[1];
    	myProblem_id = (string)row2[2];
    	myIn_date = (string)row2[3];
	myResult = (string)row2[4];
	myLanguage = (string)row2[5];
  	//  cout<<myUser_id<<endl<<myContest_id<<endl<<myProblem_id<<endl<<myIn_date<<endl;
    	if(myContest_id != "0"){//is a contest
  	     	string myStart_time,myEnd_time;
        	query_cmd = "select start_time,end_time from contest where contest_id="+myContest_id;
        	mysql_query(connection,query_cmd.c_str());
        	result2 = mysql_store_result(connection);
        	row2 = mysql_fetch_row(result2);
        	myStart_time = (string)row2[0];
        	myEnd_time = (string)row2[1];
       		// cout<<myStart_time<<endl<<myEnd_time<<endl;
        	int  mySubmittime = myDiff(myIn_date,myStart_time);
        	//cout<<myAddtime<<endl;
		
        	if(myDiff(myIn_date,myEnd_time) <= 0 && mySubmittime > 0){//between the period
          		//cout<<"haha"<<endl;
			//update stat infomation
            		query_cmd = "select count(*) from conteststat where problem_id="+myProblem_id+
					" and contest_id="+myContest_id;
			mysql_query(connection,query_cmd.c_str());
			//cout<<query_cmd<<endl;
			result2 = mysql_store_result(connection);
			row2 = mysql_fetch_row(result2);
			string myStatNum;
			if(row2 == NULL){
				myStatNum = "0";
			}
			else myStatNum = (string)row2[0];
			//cout<<myStatNum<<endl;
			if(myStatNum == "0"){
				query_cmd = "insert into conteststat(contest_id,problem_id) values("+
				myContest_id+","+myProblem_id+")";
				//cout<<query_cmd<<endl;
				mysql_query(connection,query_cmd.c_str());
			}
			int myIntResult = atoi(myResult.c_str()),myIntLanguage = atoi(myLanguage.c_str());
			string resultStr = getResultStr(myIntResult);
			string languageStr = getLanguageStr(myIntLanguage);
			
			int myFlag = 0;
			if(myIntResult >= 0 && myIntResult <= 12){
				myFlag = 1;
				query_cmd = "update conteststat set "+resultStr+"="+resultStr+"+1 where 	contest_id="+myContest_id+" and problem_id="+myProblem_id;
				mysql_query(connection,query_cmd.c_str());
			}
			if(myIntLanguage >= 0 && myIntLanguage <= 3){
			   myFlag = 1;
			   query_cmd = "update conteststat set "+languageStr+"="+languageStr+"+1 where contest_id="
				+myContest_id+" and problem_id="+myProblem_id;
			   mysql_query(connection,query_cmd.c_str());
			}
			if(myFlag == 1){
			    query_cmd = "update conteststat set Total=Total+1 where contest_id="+myContest_id+
				" and problem_id="+myProblem_id;
			    mysql_query(connection,query_cmd.c_str());
			}
			
			
			
			
            		query_cmd = "select num from contest_problem where contest_id="+myContest_id+" and problem_id=" + myProblem_id; //requires num to be distinct;
             			                                                    
            		mysql_query(connection,query_cmd.c_str());
            		result2 = mysql_store_result(connection);
            		row2 = mysql_fetch_row(result2);
            		string myNum = (string)row2[0];
            		int myProblemNum;
            		query_cmd = "select count(*) from contest_problem where num <"
            			+myNum+" and contest_id=" + myContest_id;
			//cout<<query_cmd<<endl;
            		mysql_query(connection,query_cmd.c_str());
            		result2 = mysql_store_result(connection);
            		row2 = mysql_fetch_row(result2);
            		myProblemNum = atoi(((string)row2[0]).c_str());
           	      //  cout<<myNum<<endl<<myProblemNum<<endl;
            		query_cmd=" select nick from user where user_id=\'" + myUser_id+"\'";
            		mysql_query(connection,query_cmd.c_str());
            		result2 = mysql_store_result(connection);
            		row2 = mysql_fetch_row(result2);
            		if(row2 != NULL) myNick = (string)row2[0];
            		//cout<<myNick<<endl;
			//decide whether the user has been in the acmattend table 
            		else myNick="";
            		query_cmd = "select count(*) from acmattend where user_id=\'"+myUser_id+"\' and contest_id="+myContest_id;
            		mysql_query(connection,query_cmd.c_str());
            		result2 = mysql_store_result(connection);
            		row2 = mysql_fetch_row(result2);
	                myUserNum = (string)row2[0];
           		cout<<"hehe"<<myUserNum<<endl;
            		if(myUserNum == "0"){
              		  	query_cmd = "insert into acmattend(user_id,nick,contest_id) values(\'"
                			+myUser_id+"\',\'"+myNick+"\',"+myContest_id+")";
				//cout<<query_cmd<<endl;
                		mysql_query(connection,query_cmd.c_str());
               		//cout<<query_cmd<<endl;
            		}
			else{
				query_cmd= "update acmattend set nick = \'"+myNick+"\' where user_id=\'"+myUser_id+"\' and contest_id="
                                          +myContest_id;
				mysql_query(connection,query_cmd.c_str());
			}
                                
            		string myTimestr = getMyTimestr(myProblemNum);
            		string wrongSubmitstr = getWrongSubmitstr(myProblemNum);
            		int wrongSubmit = (acceptnum == 1)?0:1;
	    		string strAcceptnum = (acceptnum == 1)?"1":"0";
            		query_cmd="select "+wrongSubmitstr+","+myTimestr+" from acmattend where user_id=\'"+myUser_id+"\' and contest_id="+myContest_id;
          		// cout<<query_cmd<<endl;
            		mysql_query(connection,query_cmd.c_str());
            		result2 = mysql_store_result(connection);
            		row2=mysql_fetch_row(result2);
            		int oriSubmit = atoi(((string)row2[0]).c_str());
           	        int oriTime = atoi(((string)row2[1]).c_str());
			if(oriSubmit >= 10000){ //has AC that problem before
			}
			else{
				
             			if(acceptnum == 1){//AC the problem and then we calculate new penalty
					addpenaltytime = 1200*oriSubmit + mySubmittime;
					wrongSubmit += 10000;
				}
	
				else{
				       addpenaltytime = 0;
				}
				char strPenaltytime[15],strSubmitTime[15];
				char myWrongSubmitNum[15];
     				sprintf(strPenaltytime,"%d",addpenaltytime);
				sprintf(strSubmitTime,"%d",mySubmittime);
				sprintf(myWrongSubmitNum,"%d",wrongSubmit); 
            			query_cmd = "update acmattend set accepts=accepts+"+strAcceptnum+
              		  		  ",penalty=penalty+"+(string)strPenaltytime+","+myTimestr+"="+(string)strSubmitTime+","
               		   		+wrongSubmitstr+"="+wrongSubmitstr+"+"+(string)myWrongSubmitNum+" where user_id=\'"+myUser_id
                	  		+"\' and contest_id="+myContest_id;

                        	//cout<<query_cmd<<endl;
            			mysql_query(connection,query_cmd.c_str());
			}
         	}
         
   	}
 	 mysql_free_result(result);
  	 mysql_free_result(result2);
  	 mysql_close(connection);

  	 return ;
}


string getMyTimestr(int num)
{
    string str = "__time";
    char ch = (char)('A'+num);
    str[0]=ch;
    return str;
}

string getResultStr(int result)
{
	string strArray[15]={"AC","PE","TLE","MLE","WA","OLE","CE","RE"};
	if(result >= 0 && result <= 6) return strArray[result];
	if(result >= 7 && result <= 12) return strArray[7];
}

string getLanguageStr(int language)
{
	string strArray[3]={"C_Cpp","Java","Pascal"};
	if(language == 0 || language == 1) return strArray[0];
	return strArray[language-1];
}


string getWrongSubmitstr(int num)
{
   string str="__WrongSubmits";
   char ch = (char)('A'+num);
   str[0] = ch;
   return str;
}

//It assume that contest is held in a month
int myDiff(string end_time,string start_time)
{
  //  char* next = end_time.c_str(),first = start_time.c_str();
    int year1,month1,day1,hour1,minute1,second1;
    int year2,month2,day2,hour2,minute2,second2;
    sscanf(end_time.c_str(),"%d-%d-%d %d:%d:%d",&year1,&month1,&day1,&hour1,&minute1,&second1);
    sscanf(start_time.c_str(),"%d-%d-%d %d:%d:%d",&year2,&month2,&day2,&hour2,&minute2,&second2);
    int diff = (day1-day2)*3600*24+(hour1-hour2)*3600+(minute1-minute2)*60+
                (second1 - second2);
    return diff;
}

//knuthocean end
int main(int agrc,char* argv[]){

	int status,language,time_limit,case_time_limit,memory_limit;
	string problem_id;
	string input_path;
	string solution_id;
	run_info r_info={ 0,0,0,"","","" };

	solution_id=(string)argv[1];
	/* get_info(	const string solution_id,
			string &prcoutoblem_id,
			string &input_path,
			int &language,
			int &time_limit,
			int &case_time_limit,
			int &memory_limit)
	*/
	if(get_info(solution_id,problem_id,input_path,language,time_limit,case_time_limit,memory_limit)){
		r_info.status=ERROR_INT;
		//printf("%s\n",status_map[r_info.status].c_str());
	}
	else {
		status=WOJ_JUDGE(
				r_info,
				input_path,
				source_output_path,
				solution_id,
				problem_id,
				language,
				memory_limit,
				time_limit,
				case_time_limit
			);
		r_info.status=status;
		printf("time = %d S\nmemory = %d\n%s\n",r_info.run_time,r_info.run_memory,status_map[r_info.status].c_str());
		//if(r_info.CE_info!="") 
		//	printf("%s",r_info.CE_info.c_str());
		//if(r_info.JAVA_RE_info!="")
		//	printf("%s",r_info.JAVA_RE_info.c_str());
		//if(r_info.PAS_RE_info!="") printf("%s\n",r_info.PAS_RE_info.c_str());
		
	}
	update_user(r_info,solution_id);
	//cout<<"fuck"<<endl;
	//remove(source_output_path.c_str());

	return 0;
}


