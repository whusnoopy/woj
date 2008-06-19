#include "newslistprocessimp.h"

#include <string>
#include <vector>
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "object/contest.h"
#include "object/list.h"
#include "data/datainterface.h"
using namespace std;

void NewsListProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the News List data for :" << ip;
  NewsList news;
  NewsInfo news_info;
  news_info.title = string("NULL");
  news_info.page_id = 0;
  news = DataInterface::getInstance().getNewsList(news_info);
  string data;
  char sep = 1;
  bool first = true;
  NewsList::iterator iter_news = news.begin();
  while (iter_news != news.end()) {
    if (first)
      first = false;
    else
      data += sep;
    data += stringPrintf("%d", iter_news->news_id);
    data += sep + iter_news->title;
    data += sep + iter_news->time;
    iter_news++;
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
  LOG(INFO) << "Process News List completed for:" << ip;
}

