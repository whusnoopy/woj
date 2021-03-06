#ifndef _FLOOD_SERVER_DATA_LIST_H__
#define _FLOOD_SERVER_DATA_LIST_H__
  
#include <string>
#include <vector>
#include <map>
#include <set>
#include <deque>

#include "file.h"
#include "status.h"
#include "class.h"
using namespace std;

typedef struct _USER_LIST_ITEM_{
	string user_id;
	string nickname;
	string last_login_time;
	string last_login_ip;
	string reg_time;
}UserListItem;

typedef struct _PROBLEM_USER_AC_{
  string user_id;
  int problem_id;
}ProblemUserAC;

typedef struct _PROBLEM_LIST_ITEM_{
  int problem_id;
  string title;
  string source;
  int accepted;
  int submit;
  int ac;
  bool available;
}ProblemListItem;

typedef struct _CONTEST_LIST_ITEM_{
  string title;
  string start_time;
  string end_time;
  string type;
  int public_id;
  int contest_id;
  bool available;
}ContestListItem;

typedef struct _CONTEST_PROBLEM_TIME_{
	int in_contest_id;
	int problem_id;
  int penalty;
  int submit;
  bool ac;
}ContestProblemTime;

typedef struct _CONTEST_RANK_LIST_ITEM_{
  string user_id;
  string nickname;
  int accepted;
  int penalty;
  map<int, ContestProblemTime> problem_penalty;
}ContestRankListItem;

typedef struct _RANK_LIST_ITEM_{
  string user_id;
  string nickname;
  int solved;
  int submit;
}RankListItem;

typedef struct _DISCUSS_LIST_ITEM_{
  int discuss_id;
  string title;
  string date;
  string user_id;
  int reply_id;
  int topic_id;
  int problem_id;
  int contest_id;
}DiscussListItem;

typedef struct _MAIL_LIST_ITEM_{
  int mail_id;
  string to_user;
  string from_user;
  string title;
  string date;
  bool read;
}MailListItem;

typedef struct _NEWS_LIST_ITEM_{
  int news_id;
  string title;
  string time;
}NewsListItem;

typedef struct _CONTEST_STATISTICS_ITEM_{
	int problem_id;
	int incontest_id;
  int AC;
  int PE;
  int CE;
  int WA;
  int TLE;  
  int RE;    
  int MLE;    
  int OLE;    
  int Total;    
  int C_CPP;    
  int Java;    
  int Pascal;
}ContestStatisticsItem;

typedef struct _PROBLEM_STATISTICS_ITEM_{
	int problem_id;
  int AC;
  int PE;
  int CE;
  int WA;
  int TLE;  
  int RE;    
  int MLE;    
  int OLE;    
  int Total;
  int User_Total;
  int User_AC;
  int C_CPP;    
  int Java;    
  int Pascal;
}ProblemStatistics;

typedef struct _LINK_LIST_ITEM_{
  string url;
  string site_name;
}LinkListItem;

typedef struct _CONTEST_PROBLEM_ITEM_{
  int problem_id;
  int in_contest_id;
  string title;
  int accepted;
  int total;
}ContestProblemItem;

typedef struct _JUDGE_MISSION_{
  Status status; 
  string source;
  int version;
  map<string, string> in_and_out_path;
  string spj_source_path;
  int time_limit;
  int case_time_limit;
  int memory_limit;
  string data_path;
  bool spj;
}JudgeMission;

typedef struct _CONTEST_INFO_ITEM_ {
  int contest_id;
  string title;
  vector<int> problem_list;
}ContestInfoItem;

typedef vector<int> ProblemIdList;
typedef vector<ContestInfoItem> ContestInfoList;
typedef vector<UserListItem> UserList;
typedef vector<ProblemListItem> ProblemList;
typedef vector<ContestListItem> ContestList;
typedef vector<ContestRankListItem> ContestRankList;
typedef vector<RankListItem> RankList;
typedef vector<DiscussListItem> DiscussList;
typedef vector<MailListItem> MailList;
typedef vector<NewsListItem> NewsList;
typedef vector<ContestStatisticsItem> ContestStatistics;   
typedef vector<File> FileList;   
typedef vector<Status> StatusList;
typedef vector<LinkListItem> LinkList;
typedef set<int> ProblemSet;
typedef set<string> UserSet;
typedef vector<ContestProblemItem> ContestProblemList;
typedef set<ProblemUserAC> UserACSet;
typedef vector<int> TopicSet;
typedef deque<JudgeMission> JudgeQueue;
typedef vector<Class> ClassList;
typedef map<int, Status> RejudgeSet;

bool operator<(const ProblemUserAC& a, const ProblemUserAC& b);
bool operator<(const ContestRankListItem& a, const ContestRankListItem& b);

#endif /*LIST_H_*/
