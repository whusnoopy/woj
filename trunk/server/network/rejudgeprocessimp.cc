#include "rejudgeprocessimp.h"

#include <vector>
#include <string>

#include "object/objectinc.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "judgecontrol/judgecontrol.h"
#include "base/judge_result.h"
using namespace std;

void RejudgeProcessImp::process(int socket_fd, const string& ip, int length) {
  LOG(INFO) << "Process the Rejudge for:" << ip;
  char* buf;
  buf = new char[length+1];
  memset(buf,0,sizeof(buf));
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string data(buf, buf + length);
  delete[] buf;
  vector<string> datalist;
  spriteString(data, 1, datalist);
  vector<string>::iterator iter = datalist.begin();
  Status status;
  int status_id;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find status_id from data for:" << ip;
    return;
  }
  status_id = atoi(iter->c_str());
  iter++;
  if (status_id <= 0) {
    sendReply(socket_fd, 'N');
    LOG(ERROR) << "status_id can not be nagative";
    return ;
  }
  status = DataInterface::getInstance().getStatus(status_id);
  if (status.getResult() == ACCEPTED) {
    status.setResult(0);
    DataInterface::getInstance().updateStatus(status);
    DataInterface::getInstance().updateUserSolved(status, -1);
    DataInterface::getInstance().addProblemSolved(status.getProblemId(), -1);
    DataInterface::getInstance().addProblemUserSolved(status, -1);
  }
  status.setErrorId(0);
  Problem problem = DataInterface::getInstance().getProblem(status.getProblemId());
  string source = DataInterface::getInstance().getCode(status.getCodeId()).getCodeContent();
  JudgeMission mission;
  mission.status = status;
  mission.source = source;
  mission.version = problem.getVersion();
  mission.time_limit = problem.getTimeLimit();
  mission.case_time_limit = problem.getCaseTimeLimit();
  mission.memory_limit = problem.getMemoryLimit();
  mission.spj = problem.getSpj();
  mission.in_and_out_path = DataInterface::getInstance().getProblemInAndOutFile(problem);
  if (mission.spj) 
    mission.spj_source_path = DataInterface::getInstance().getProblemSpjFile(problem);
  mission.data_path = getProblemDataPath(problem.getProblemId()); 
  JudgeControl::getInstance().addMission(mission);
  if (sendReply(socket_fd, 'Y') != 0) {
    LOG(ERROR) << "Send reply error:" << ip;
    return;
  }
  LOG(INFO) << "Submit Rejudge completed." << ip;
}

