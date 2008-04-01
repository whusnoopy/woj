#include "ableproblemprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "data/datainterface.h"
#include "util/calulate.h"
#include "object/problem.h"
#include "object/list.h"
#include "object/info.h"
#include "object/user.h"
using namespace std;

void AbleProblemProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process able problem for:" << ip;
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
  Problem problem;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find problem_id from data for:" << ip;
    return;
  }
  problem.setProblemId(atoi(iter->c_str()));
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find available from data for:" << ip;
    return;
  }
  problem.setAvailable(*iter == "Y");
  int ret = 0;
  ret = DataInterface::getInstance().disableProblem(problem);
  if (ret) {
    sendReply(socket_fd, 'N');
    return;
  }

  if (sendReply(socket_fd, 'Y')) {
    LOG(ERROR) << "Cannot return problem_id to:" << ip;
    return;
  }

  LOG(ERROR) << "Process able problem completed for:" << ip;
}

