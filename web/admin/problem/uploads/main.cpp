# include "mysql.h"
# include "woj.h"

#define RESULTNUM  13  //contestÍ³ŒÆÊ±ÐèÒªÍ³ŒÆµÄœá¹ûÀàÐÍ 
#define LANGUAGENUM 4  //contestÍ³ŒÆÊ±ÐèÒªÍ³ŒÆµÄÓïÑÔÀàÐÍ 
string getMyTimestr(int);
string getWrongSubmitstr(int);
int myDiff(string,string);
string getResultStr(int);
string getLanguageStr(int);
void updateACMContest(run_info,string,int,int);
int rejudgeFirst = 0,rejudgeLast = 0;

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
	// æsqläž­çsource_codeäž­çsourceèœ¬åäžºçšåºæä»¶
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
  //printf("hehe%s\n",solution_id.c_str());
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
       
	//cout<<problem_id<<endl<<input_path<<endl<<time_limit<<endl<<case_time_limit<<endl<<memory_limit<<endl;	
 	mysql_free_result(result);
 	mysql_close(connection);
	return 0;
}
char cmd[1<<15];
void update_user(run_info user_info,string solution_id,bool rejudge){

	MYSQL_RES *result,*result2;
	MYSQL_ROW row,row2;
	MYSQL *connection,mysql;
	int state;
	string query_cmd;
	char time[50];
	char memory[50];
	char uresult[50];
	FILE *fp;
	//æintèœ¬äžºchar *ç±»å
	sprintf(time,"%d",user_info.run_time);
	sprintf(memory,"%d",user_info.run_memory);
	sprintf(uresult,"%d",user_info.status);
	cout<<time<<endl<<memory<<endl<<uresult<<endl;
	//åè¯åmysql
	mysql_init(&mysql);
	connection = mysql_real_connect(&mysql, HOST, USER, PASSWORD, DATABASE, 0, NULL, 0);
	//æ¥æŸproblem_id
	query_cmd="SELECT problem_id,user_id,result FROM solution WHERE solution_id="+solution_id;
	//cout<<query_cmd.c_str()<<endl;
	mysql_query(connection,query_cmd.c_str());
	result=mysql_store_result(connection);
	row=mysql_fetch_row(result);//row[0]å°±æ¯problemäº,row[1]å°±æ¯userã
	int oriResult = atoi(((string)row[2]).c_str()); //rejudgeÇ°µÄÆÀÅÐœá¹û 

    //µÃµœACÄ³žöproblemµÄŽÎÊý 
	query_cmd="select count(*) from solution where user_id = \'"+(string)row[1]+"\' and problem_id="+(string)row[0]+" and result=0";
    mysql_query(connection,query_cmd.c_str());
	result2=mysql_store_result(connection);
    row2 = mysql_fetch_row(result2);

	query_cmd="UPDATE solution SET time="+(string)time+",memory="+(string)memory+",result="+(string)uresult+" WHERE solution_id="+solution_id;
	mysql_query(connection,query_cmd.c_str());
	if(!rejudge){
         query_cmd="UPDATE problem SET submit = submit + 1 WHERE problem_id="+(string)row[0];
         mysql_query(connection,query_cmd.c_str());
         query_cmd="UPDATE user SET submit = submit + 1 WHERE user_id= \'"+(string)row[1]+"\'";
         mysql_query(connection,query_cmd.c_str());
    }    
	
	if(user_info.status==0 &&(!rejudge || oriResult != 0)){ //AC++
		query_cmd="UPDATE problem SET accepted = accepted + 1 WHERE problem_id="+(string)row[0];
		mysql_query(connection,query_cmd.c_str());
                if((string)row2[0] == "0"){
			query_cmd="UPDATE user SET solved = solved + 1 WHERE user_id= \'"+(string)row[1]+"\'";
			mysql_query(connection,query_cmd.c_str());
		}
	}
	if(user_info.status != 0 && rejudge && oriResult == 0){ //Ô­ÀŽAC,ÏÖÔÚŽí
               query_cmd="UPDATE problem SET accepted = accepted - 1 WHERE problem_id="+(string)row[0];
               mysql_query(connection,query_cmd.c_str());
               if((string)row2[0] == "1"){
 			query_cmd="UPDATE user SET solved = solved - 1 WHERE user_id= \'"+(string)row[1]+"\'";
			mysql_query(connection,query_cmd.c_str());
	       }
    }    			
       
	//ÐŽCE,JAVARE,PASCALRE±í 
	if(user_info.CE_info!=""){ //CE
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
	
	 updateACMContest(user_info,solution_id,rejudge,oriResult);
 	 mysql_free_result(result);
  	 mysql_free_result(result2);
  	 mysql_close(connection);
  	 return ;
}

void updateACMContest(run_info user_info,string solution_id,int rejudge,int oriResult)
{
    MYSQL_RES *result2;
    MYSQL_ROW row2;
    MYSQL *connection,mysql;
    string query_cmd;
    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql, HOST, USER, PASSWORD, DATABASE, 0, NULL, 0);
    int acceptnum,addpenaltytime;
    acceptnum = (user_info.status == 0) ? 1:0; //whether the problem AC
 	
 	//µÃµœsolution_id¶ÔÓŠµÄsolutionÏà¹ØÐÅÏ¢ 
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
			        string oriResultStr = getResultStr(oriResult);
		            string languageStr = getLanguageStr(myIntLanguage);
			
			        int myFlag = 0;
			        if(rejudge){
                        if(oriResult >= 0 && oriResult < RESULTNUM){
                             query_cmd = "update conteststat set "+oriResultStr+"="+oriResultStr+"-1 where 	contest_id="+myContest_id+" and problem_id="+myProblem_id;
				             mysql_query(connection,query_cmd.c_str());
				         }
		            }        
			        if(myIntResult >= 0 && myIntResult < RESULTNUM){
				         myFlag = 1;
				         query_cmd = "update conteststat set "+resultStr+"="+resultStr+"+1 where contest_id="+myContest_id+" and problem_id="+myProblem_id;
				         mysql_query(connection,query_cmd.c_str());
			        }
			        if(myIntLanguage >= 0 && myIntLanguage < LANGUAGENUM){
			             myFlag = 1;
			             if(!rejudge){
                             query_cmd = "update conteststat set "+languageStr+"="+languageStr+"+1 where contest_id="
				                       +myContest_id+" and problem_id="+myProblem_id;
			                 mysql_query(connection,query_cmd.c_str());
			             }    
                   }
			       if(myFlag == 1 && !rejudge){
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
			    mysql_query(connection,query_cmd.c_str());
                result2 = mysql_store_result(connection);
                row2 = mysql_fetch_row(result2);
                myProblemNum = atoi(((string)row2[0]).c_str());
                query_cmd=" select nick from user where user_id=\'" + myUser_id+"\'";
                mysql_query(connection,query_cmd.c_str());
                result2 = mysql_store_result(connection);
                row2 = mysql_fetch_row(result2);
           	    //decide whether the user has been in the acmattend table 
                if(row2 != NULL && row2[0] != NULL) myNick = (string)row2[0];
                else myNick="";
                query_cmd = "select count(*) from acmattend where user_id=\'"+myUser_id+"\' and contest_id="+myContest_id;
                mysql_query(connection,query_cmd.c_str());
                result2 = mysql_store_result(connection);
                row2 = mysql_fetch_row(result2);
                myUserNum = (string)row2[0];
                if(myUserNum == "0"){
       		  	    query_cmd = "insert into acmattend(user_id,nick,contest_id) values(\'"
                			    +myUser_id+"\',\'"+myNick+"\',"+myContest_id+")";
				    mysql_query(connection,query_cmd.c_str());
               	
   	            }
			    else{
				    query_cmd= "update acmattend set nick = \'"+myNick+"\' where user_id=\'"+myUser_id+"\' and contest_id="
                               +myContest_id;
				    mysql_query(connection,query_cmd.c_str());
			    }
                string myTimestr = getMyTimestr(myProblemNum);
                string wrongSubmitstr = getWrongSubmitstr(myProblemNum);
                
                query_cmd="select "+wrongSubmitstr+","+myTimestr+" from acmattend where user_id=\'"+myUser_id+"\' and contest_id="+myContest_id;
          	    mysql_query(connection,query_cmd.c_str());
          	    result2 = mysql_store_result(connection);
          	    row2=mysql_fetch_row(result2);
          	    int oriSubmit = atoi(((string)row2[0]).c_str());
  	            int oriTime = atoi(((string)row2[1]).c_str());
  	            int myType,wrongSubmit;
  	            string strAcceptnum;
  	            
  	            myType = 2;
  	            if(!rejudge){ //�����ж� 
  	                wrongSubmit = oriSubmit;
  	                if(wrongSubmit < 10000){
                        if(acceptnum == 1){//AC a new problem and then we calculate new penalty
					       myType = 0;
					       wrongSubmit += 10000;
                           addpenaltytime = 1200*(wrongSubmit%10000) + mySubmittime;
					       strAcceptnum = "1";
                        }
                        else{
                            myType = 1;
                            wrongSubmit++;
                        }    
                    }    
                }
                                      
			  
                char strPenaltytime[15],strSubmitTime[15];
                char myWrongSubmitNum[15];
                sprintf(strPenaltytime,"%d",addpenaltytime);
                sprintf(strSubmitTime,"%d",mySubmittime);
                sprintf(myWrongSubmitNum,"%d",wrongSubmit); 
                if(myType == 0){ // �޸�contestStanding��Ϣ 
                    query_cmd = "update acmattend set accepts=accepts+"+strAcceptnum+
		  		          ",penalty=penalty+"+(string)strPenaltytime+","+myTimestr+"="+(string)strSubmitTime+","
		  		          +wrongSubmitstr+"="+(string)myWrongSubmitNum+" where user_id=\'"+myUser_id
                          +"\' and contest_id="+myContest_id;
                }
                else if(myType == 1){ //ֻ�ı�wrongSubmit 
                    query_cmd = "update acmattend set "
		  		          +wrongSubmitstr+"="+(string)myWrongSubmitNum+" where user_id=\'"+myUser_id
                          +"\' and contest_id="+myContest_id;
                }    
                if(myType >= 0 && myType <= 1 ) mysql_query(connection,query_cmd.c_str());
                
		// cout<<rejudge<<"   "<<rejudgeLast<<endl;
                if(rejudge && rejudgeLast){ //rejudge�����Standing�����Ϣ
                    query_cmd = "select solution_id,in_date from solution where result = 0 and user_id=\'"+ myUser_id
                             +"\' and problem_id="+myProblem_id+" and date_sub(in_date,interval 0 day)>timestamp(\'"
                             +myStart_time+"\') and date_sub(in_date,interval 0 day)<timestamp(\'"+myEnd_time+"\')";
                    cout<<query_cmd<<endl;
                    mysql_query(connection,query_cmd.c_str());
          	        result2 = mysql_store_result(connection);
      	            row2=mysql_fetch_row(result2);
                    string rejudgeSolution_id = "0",rejudgeIn_date = "0";
                    if(row2 != NULL){
                        rejudgeSolution_id = (string)row2[0];
                        rejudgeIn_date = (string)row2[1];
                    }
                    string strRejudgeTime,strRejudgeWrongSubmit;
                    int intRejudgeTime = myDiff(rejudgeIn_date,myStart_time);
                     char rejudgeTempstr[30];
                     int rejudgeTempint;
                    if(rejudgeSolution_id == "0" || myDiff(rejudgeIn_date,myEnd_time) > 0 || intRejudgeTime <= 0){
                        strRejudgeTime = "0";
                        query_cmd = "select count(*) from solution where user_id=\'" + myUser_id
                                   +"\' and problem_id="+myProblem_id+" and date_sub(in_date,interval 0 day)>timestamp(\'"
                                   +myStart_time+"\') and date_sub(in_date,interval 0 day)<timestamp(\'"+myEnd_time+"\')";
                        mysql_query(connection,query_cmd.c_str());
                        cout<<query_cmd<<endl;
                        result2 = mysql_store_result(connection);
                        row2 = mysql_fetch_row(result2);
                        strRejudgeWrongSubmit = row2[0];
                    }
                    else{
                       
                        sprintf(rejudgeTempstr,"%d",intRejudgeTime);
                        strRejudgeTime = (string)rejudgeTempstr;
                        query_cmd = "select count(*) from solution where result != 0 and user_id=\'"+myUser_id
                        +"\' and problem_id="+myProblem_id+" and solution_id <"+rejudgeSolution_id;
                        cout<<query_cmd<<endl;
                        mysql_query(connection,query_cmd.c_str());
                        result2 = mysql_store_result(connection);
                        row2 = mysql_fetch_row(result2);
                        rejudgeTempint = atoi(((string)row2[0]).c_str()) + 10000;
                        sprintf(rejudgeTempstr,"%d",rejudgeTempint);
                        strRejudgeWrongSubmit = (string)rejudgeTempstr;
                    }
                    //����acmattend ����myProblem_id �����Ϣ
                    query_cmd = "update acmattend set "+myTimestr+"="+strRejudgeTime+","+
		  		          wrongSubmitstr+"="+ strRejudgeWrongSubmit +" where user_id=\'"+myUser_id
                          +"\' and contest_id="+myContest_id;  
                    cout<<query_cmd<<endl;
                    mysql_query(connection,query_cmd.c_str());
                    
                    //����accepts��penalty 
                    query_cmd =   "select A_time,A_WrongSubmits,B_time,B_WrongSubmits,C_time,C_WrongSubmits,D_time,D_WrongSubmits,E_time,E_WrongSubmits,F_time,F_WrongSubmits,G_time,G_wrongSubmits,H_time,H_WrongSubmits,I_time,I_WrongSubmits,J_time,J_WrongSubmits,K_time,K_WrongSubmits,L_time,L_WrongSubmits,M_time,M_WrongSubmits,N_time,N_WrongSubmits from acmattend  where user_id=\'"+myUser_id
                          +"\' and contest_id="+myContest_id;  
                    mysql_query(connection,query_cmd.c_str());
                    cout<<query_cmd<<endl;
                    result2 = mysql_store_result(connection);
                    row2 = mysql_fetch_row(result2);
                    int rejudgeIndex,rejudgeAccept=0,rejudgePenalty=0;
                    string strRejudgeAccept,strRejudgePenalty;
                    for(rejudgeIndex=0;rejudgeIndex<28;rejudgeIndex++){
                          int tempTime,tempSubmit;
                          if(rejudgeIndex%2 == 0){
                              tempTime = atoi(((string)row2[rejudgeIndex]).c_str());
                          }    
                          else{
                              tempSubmit = atoi(((string)row2[rejudgeIndex]).c_str());
                              if(tempSubmit >= 10000){
                                   rejudgeAccept++;
                                   rejudgePenalty = rejudgePenalty + (tempSubmit%10000)*1200 + tempTime;
                               }     
                          }    
                          
                    }
                    sprintf(rejudgeTempstr,"%d",rejudgeAccept);
                    strRejudgeAccept = (string)rejudgeTempstr;
                    sprintf(rejudgeTempstr,"%d",rejudgePenalty);
                    strRejudgePenalty = (string)rejudgeTempstr;
                    
                    //����acmattend ��accept��penalty��Ϣ
                   query_cmd = "update acmattend set accepts="+strRejudgeAccept+
		  		          ",penalty="+strRejudgePenalty+" where user_id=\'"+myUser_id
                          +"\' and contest_id="+myContest_id;
                   cout<<query_cmd<<endl;
                 
                   mysql_query(connection,query_cmd.c_str());
                    
               } 
	            
         }
				
  	}
	
    mysql_free_result(result2);
 
    mysql_close(connection);
   
    	
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

//µ÷ÓÃÐÎÊœ:
//main solution_id
//main rejudge from_id to_id problem_id //¶Ôfrom_idºÍto_idÖ®ŒäµÄÄ³žöproblem_idœøÐÐrejudge
int main(int argc,char* argv[]){

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
	int rejudge = 0;
	string argProblem_id = "0",from_id = "0",to_id = "0";
	if(solution_id=="rejudge"){
	    rejudge = 1;
            from_id = (string)argv[2];
            to_id = (string)argv[3];
            argProblem_id = (string)argv[4];
        }
        else{
	  rejudge = 0;
          to_id = from_id = solution_id;
	}                 
	
    int intFrom_id = atoi(from_id.c_str()),intTo_id = atoi(to_id.c_str());
    if(intFrom_id > intTo_id){
        intFrom_id ^= intTo_id ^= intFrom_id ^= intTo_id;
    }        
    int intSolu_id;
    char strSolu_id[20];
    // cout<<intFrom_id<<endl<<intTo_id<<endl;
    for(intSolu_id = intFrom_id; intSolu_id <= intTo_id; intSolu_id++){ 
        if(intSolu_id == intFrom_id) rejudgeFirst = 1;
        else rejudgeFirst = 0;
        if(intSolu_id == intTo_id) rejudgeLast = 1;
        else rejudgeLast = 0;
       sprintf(strSolu_id,"%d",intSolu_id);  
       //cout<<"hehe:"<<strSolu_id<<endl;
	   if(get_info((string)strSolu_id,problem_id,input_path,language,time_limit,case_time_limit,memory_limit)){
		    r_info.status=ERROR_INT;
		    //printf("%s\n",status_map[r_info.status].c_str());
		    // cout<<"hehe"<<endl;
       }
	   else {
	   
	        if(argProblem_id != "0" && argProblem_id != problem_id){
                   	remove(source_output_path.c_str());
	                if(language == 2){
		             string tmp_cmd="rm -r "+WORK_PATH+"/"+strSolu_id;
		             system(tmp_cmd.c_str());
		//remove((WORK_PATH+"/"+solution_id+"/Main.class").c_str());
		//rmdir((WORK_PATH+"/"+solution_id).c_str());
	                } 
                        continue;
		}
	        status=WOJ_JUDGE(
				  r_info,
				  input_path,
				  source_output_path,
				  (string)strSolu_id,
				  problem_id,
				  language,
				  memory_limit,
				  time_limit,
				  case_time_limit
			);
		    r_info.status=status;
		    printf("solution_id= %s time = %d S\nmemory = %d\n%s\n",strSolu_id,r_info.run_time,r_info.run_memory,status_map[r_info.status].c_str());
		    if(r_info.CE_info!="") 
		    	printf("%s",r_info.CE_info.c_str());
		    //if(r_info.JAVA_RE_info!="")
		    //	printf("%s",r_info.JAVA_RE_info.c_str());
		    //if(r_info.PAS_RE_info!="") printf("%s\n",r_info.PAS_RE_info.c_str());
	   }
       update_user(r_info,(string)strSolu_id,rejudge);
       //remove(source_output_path.c_str());
	}    

	return 0;
}


