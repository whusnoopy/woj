#include "homepageprocessimp.h"

#include <string>
#include <vector>
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "../object/contest.h"
#include "../object/list.h"
using namespace std;

void HomePageProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the homepage data for :" << ip;
  Contest contest;
  UserList most_diligent_programmer;
  NewsList news;
  //contest = DatabaseInterface::getInstance().getUpcomingContest();
  //most_diligent_programmer = DatabaseInterface::getInstance().getMostDiligentUser();
  //links = FileInterface::getInstance().getLink();
  //news = DatabaseInterface::getInstance().getNews();
  string data;
  char sep = 1;
  data = contest.getTitle() + sep + contest.getStartTime();
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
  int len = data.length();
  if (socket_write(socket_fd, reinterpret_cast<char*>(&len), sizeof(len))){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, data.c_str(), data.length())) {
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  LOG(INFO) << "Process the homepage data completed for:" << ip;
}

