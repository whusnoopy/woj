#include "adddiscussprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "data/datainterface.h"
#include "util/calulate.h"
#include "object/discuss.h"
#include "object/list.h"
#include "object/info.h"
#include "object/user.h"
using namespace std;

void AddDiscussProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process add Discuss for:" << ip;
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
  Discuss discuss; 

  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find reply_id from data for:" << ip;
    return;
  }
  discuss.setReplyId(atoi(iter->c_str()));
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find topic_id from data for:" << ip;
    return;
  }
  discuss.setTopicId(atoi(iter->c_str()));
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id from data for:" << ip;
    return;
  }
  discuss.setUserId((*iter == "?")?"NULL":*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find problem_id from data for:" << ip;
    return;
  }
  discuss.setProblemId(atoi(iter->c_str()));  
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find contest_id from data for:" << ip;
    return;
  }
  discuss.setContestId(atoi(iter->c_str()));
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find title from data for:" << ip;
    return;
  }
  discuss.setTitle((*iter == "?")?"NULL":*iter);
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find content from data for:" << ip;
    return;
  }
  discuss.setContent((*iter == "?")?"NULL":*iter);
  iter++;
  discuss.setDate(getLocalTimeAsString("%Y-%m-%d %H:%M:%S"));
  discuss.setAvailable(true);

  User user;
  user = DataInterface::getInstance().getUserInfo(discuss.getUserId());
  if (user.getId() == string("NULL")) {
    sendReply(socket_fd, 'N');
    return;
  }
  int ret = 0;
  ret = DataInterface::getInstance().addDiscuss(discuss);
  if (ret) {
    sendReply(socket_fd, 'N');
    return;
  }
  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot reply to:" << ip;
    return;
  }
  LOG(ERROR) << "Process adddiscuss completed for:" << ip;
}

