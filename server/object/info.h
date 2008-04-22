#ifndef _FLOOD_SERVER_DATA_INFO_H__
#define _FLOOD_SERVER_DATA_INFO_H__

#include <string>
using namespace std;

typedef struct _CONTEST_INFO_{
  string title;
  string description;
  char type;
  int page_id;
}ContestInfo;

typedef struct _CONTEST_RANK_LIST_INFO_{
  int contest_id;
  int page_size;
  int page_id;
}ContestRankListInfo;

typedef struct _DISCUSS_INFO_{
  string title;
  int problem_id;
  int contest_id;
  string user_id;
  int topic_id;
  int page_id;
}DiscussInfo;

typedef struct _MAIL_INFO_{
  string title;
  string user_id;
  bool is_recv;
  int page_id;
}MailInfo;

typedef struct _NEWS_INFO{
  string title;
  int page_id;
}NewsInfo;

typedef struct _STATUS_INFO_{
  string user_id;
  int problem_id;
  int result;
  int contest_id;
  int language;
  bool share_code_enable;
  bool share_code;
  string type;
  int page_id;
}StatusInfo;

typedef struct _CONTEST_AC_BEFORE_ {
  string user_id;
  string time;
  int contest_id;
  int problem_id;
}ContestAcBefore;

typedef struct _PROBLEM_INFO_{
  int problem_id;
  string title;
  string source;
  int related_contest;
  int page_id;
}ProblemInfo;

typedef struct _RANK_LIST_INFO_{
  int page_id;
  int seq;
}RankListInfo;

typedef struct _USER_INFO_{
  string user_id;
  string nickname;
  int page_id;
}UserInfo;

#endif /*INFO_H_*/
