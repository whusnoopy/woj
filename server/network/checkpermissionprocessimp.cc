#include "checkpermissionprocessimp.h"

#include <string>
#include <vector>

#include "object/info.h"
#include "object/list.h"
#include "object/user.h"
#include "object/code.h"
#include "object/contest.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void CheckPermissionProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process Check Permission for:" << ip;
  char* buf;
  buf = new char[length+1];
  memset(buf,0,sizeof(buf));
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string read_data(buf, buf + length);
  delete[] buf;
  vector<string> datalist;
  spriteString(read_data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find right type from data for:" << ip;
    return;
  }
  char type = (*iter)[0];
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id from data for:" << ip;
    return;
  }
  string user_id = *iter;
  iter++;
  User user = DataInterface::getInstance().getUserInfo(user_id);
  char reply = 'N';
  int code_id = 0;
  Code code;
  int contest_id;
  Contest contest;
  switch (type) {
    case 'V':      //View code
      if (iter == datalist.end()) {
        LOG(ERROR) << "Cannot find code_id from data for:" << ip;
        return;
      }
      code_id = atoi(iter->c_str()); 
      iter++;
      code = DataInterface::getInstance().getCode(code_id);
      if (code.getCodeId() != 0) {
        bool can_read = DataInterface::getInstance().canReadCode(user_id, code_id);
        if (user.getPermission() & 0x01 || can_read)
          reply = 'Y';
      }
      if (sendReply(socket_fd, reply) != 1) {
        LOG(ERROR) << "Cannot reply to : " << ip;
        return;
      }
      break;
    case 'A':      //Admin
      if (user.getPermission() & 0x02)
        reply = 'Y';
      if (sendReply(socket_fd, reply) != 1) {
        LOG(ERROR) << "Cannot reply to : " << ip;
        return;
      }
      break;
    case 'C':      //Join Contest
      if (iter == datalist.end()) {
        LOG(ERROR) << "Cannot find code_id from data for:" << ip;
        return;
      }
      contest_id = atoi(iter->c_str());
      iter++;
      if (DataInterface::getInstance().checkPermission(contest_id, user_id))
        reply = 'Y';
      if (sendReply(socket_fd, reply) != 1) {
        LOG(ERROR) << "Cannot reply to : " << ip;
        return;
      } 
      break;
    default:
      LOG(ERROR) << "Unknown right test : " << type;
      return;
      break;
  }
  LOG(INFO) << "Process Check Permission completed for:" << ip;
}


