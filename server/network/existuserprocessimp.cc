#include "existuserprocessimp.h"

#include <string>
#include <vector>

#include "object/user.h"
#include "data/datainterface.h"
#include "base/logging.h"
#include "base/flags.h"
#include "base/util.h"
using namespace std;

void ExistUserProcessImp::process(int socket_fd, const string& ip, int length) {
  LOG(INFO) << "Process the ExistUser Process for:" << ip;
  char* buf;
  buf = new char[length + 1];
  memset(buf, 0, length + 1);
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    return;
  }
  string user_id(buf);
  delete[] buf;
  User user;
  char ret = 'Y';
  user = DataInterface::getInstance().getUserInfo(user_id);
  if (user.getId() == string("NULL")) {
    ret = 'N';
  }
  if (sendReply(socket_fd, ret)){
    LOG(ERROR) << "Cannot reply :" << ip;
    return;
  }
  LOG(INFO) << "Process ExistUser completed for:" << ip;
}

