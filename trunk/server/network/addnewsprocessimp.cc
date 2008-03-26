#include "addnewsprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "../util/calulate.h"
#include "../object/news.h"
#include "../object/list.h"
#include "../object/info.h"
#include "../object/user.h"
using namespace std;

void AddNewsProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process add Mail for:" << ip;
  char* buf;
  buf = new char[length + 1];
  memset(buf, 0, length + 1);
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string read_data(buf);
  delete[] buf;
  vector<string> datalist;
  spriteString(read_data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  News news;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find title from data for:" << ip;
    return;
  }
  news.setTitle(*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find content from data for:" << ip;
    return;
  }
  news.setContent(*iter);
  int ret = 0;
  //ret = DatabaseInterface::getInstance().addNews(news);
  if (ret) {
    sendReply(socket_fd, 'N');
    return;
  }
  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot reply to:" << ip;
    return;
  }
  LOG(ERROR) << "Process addmail completed for:" << ip;
}

