#include "problemversionprocessimp.h"

#include <string>
#include <vector>

#include "object/problem.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

void ProblemVersionProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process the Problem Version for:" << ip;
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
    LOG(ERROR) << "Cannot find problem_id from data for:" << ip;
    return;
  }
  int problem_id = atoi(iter->c_str());
  Problem problem;
  problem = DataInterface::getInstance().getProblem(problem_id);
  string databuf;
  string len = stringPrintf("%010d", 0);
  if ((problem.getProblemId() == 0)||(!problem.getAvailable())){
    socket_write(socket_fd, len.c_str(), 10);
    return;
  }
  databuf = stringPrintf("%d",problem.getVersion());
  len = stringPrintf("%010d",databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process Problem Version completed for" << ip;
}

