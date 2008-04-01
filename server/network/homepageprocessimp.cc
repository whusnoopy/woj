#include "homepageprocessimp.h"

#include <string>
#include <vector>
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "object/contest.h"
#include "object/list.h"
#include "data/datainterface.h"
using namespace std;

void HomePageProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the homepage data for :" << ip;
  ContestList contest_list;
  UserList most_diligent_programmer;
  NewsList news;
  contest_list = DataInterface::getInstance().getUpcomingContest();
  most_diligent_programmer = DataInterface::getInstance().getMostDiligenPlayer();
  //links = DataInterface::getInstance().getLink();
  NewsInfo news_info;
  news_info.title = string("NULL");
  news_info.page_id = 0;
  news = DataInterface::getInstance().getNewsList(news_info);
  string data;
  char sep = 1;
  ContestList::iterator iter_contest = contest_list.begin();
  if (iter_contest == contest_list.end()) {
    data = sep + sep;
  }else {
    data = stringPrintf("%d\001%s\001%s", 
                        iter_contest->contest_id,
                        iter_contest->title.c_str(),
                        iter_contest->start_time.c_str());
  }
  data += sep + stringPrintf("%d", most_diligent_programmer.size());
  UserList::iterator iter_user = most_diligent_programmer.begin();
  while (iter_user != most_diligent_programmer.end()) {
    data += sep + iter_user->user_id;
    iter_user++;
  }
  NewsList::iterator iter_news = news.begin();
  while (iter_news != news.end()) {
    data += sep + iter_news->title;
    data += sep + iter_news->time;
    iter_news++ ;
  }
  string len = stringPrintf("%010d",data.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, data.c_str(), data.length())) {
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  LOG(INFO) << "Process the homepage data completed for:" << ip;
}

