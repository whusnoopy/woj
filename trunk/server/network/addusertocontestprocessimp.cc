#include "addusertocontestprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "../util/calulate.h"
#include "../object/contest.h"
#include "../object/list.h"
#include "../object/info.h"
#include "../object/user.h"
using namespace std;

void AddUserToContestProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process add User to Contest for:" << ip;
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
  if (iter ==  datalist.end()) {
    LOG(ERROR) << "Cannot find contest_id from data" << ip;
    return;
  }
  Contest contest(atoi(iter->c_str()));
  iter++;
  UserSet set;
  while (iter != datalist.end()) {
    set.insert(*iter);
  }
  //DataInterface::getInstance().disableContestUsers(contest);
  int ret = 0;
  //ret = DataInterface::getInstance().addUserListtoContest(contest, set);
  if (ret) {
    sendReply(socket_fd, 'N');
    return;
  }
  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot reply to:" << ip;
    return;
  }
  LOG(ERROR) << "Process add User to Contest completed for:" << ip;
}

