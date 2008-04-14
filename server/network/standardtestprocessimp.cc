#include "standardtestprocessimp.h"

#include <vector>
#include <string>

#include "object/objectinc.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "judgecontrol/judgecontrol.h"
#include "base/judge_result.h"
using namespace std;

void StandardTestProcessImp::process(int socket_fd, const string& ip, int length) {
  LOG(INFO) << "Process the Standard Test for:" << ip;
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
  int problem_id;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find problem_id from data for:" << ip;
    return;
  }
  problem_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id from data for:" << ip;
    return;
  }
  string user_id = * iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find password from data for:" << ip;
    return;
  }
  string password = *iter;
  iter++;

  User user = DataInterface::getInstance().getUserInfo(user_id);
  if (user.getPermission() & 0x02 || user.getPassword() != password) {
    sendReply(socket_fd, 'N');
    LOG(INFO) << "Do not has right to do test standard source";
    return;
  }

  Problem problem = DataInterface::getInstance().getProblem(problem_id);
  int language;
  string source_path = DataInterface::getInstance().getProblemStandardSource(problem_id, language);
  if (source_path == "no" || language == -1) {
    sendReply(socket_fd, 'N');
    LOG(INFO) << "This is not a standard source here for " << stringPrintf("%d", problem_id);
    return;
  }
  FileData source_data = DataInterface::getInstance().getFile(source_path);
  int code_length = DataInterface::getInstance().getFileSize(source_path);
  string source(source_data.buf, source_data.buf + code_length);
  delete[] source_data.buf;
  Code code(0, false, source);
  int code_id = DataInterface::getInstance().addCode(code);
  status.setUserId(user_id);
  status.setProblemId(problem_id);
  status.setContestId(0);
  status.setTime(-1);
  status.setMemory(-1);
  status.setSubmitTime(getLocalTimeAsString("%Y-%m-%d %H:%M:%S"));
  status.setResult(0);
  status.setLanguage(language);
  status.setCodeId(code_id);
  status.setCodeLength(code_length);
  status.setSubmitIp("root");
  status.setErrorId(0);
  status.setType("S");
  status.setStatusId(DataInterface::getInstance().addStatus(status));

  JudgeMission mission;
  mission.status = status;
  mission.source = source;
  mission.version = problem.getVersion();
  mission.in_and_out_path = DataInterface::getInstance().getProblemInAndOutFile(problem_id);
  mission.time_limit = problem.getTimeLimit();
  mission.case_time_limit = problem.getCaseTimeLimit();
  mission.memory_limit = problem.getMemoryLimit();
  mission.data_path = getProblemDataPath(problem_id);
  mission.spj = problem.getSpj();
  if (mission.spj)
    mission.spj_source_path = DataInterface::getInstance().getProblemSpjFile(problem_id);

  JudgeControl::getInstance().addMission(mission);
  string status_id_str = stringPrintf("%010d", status.getStatusId());
  if (sendReply(socket_fd, 'Y') != 0) {
    LOG(ERROR) << "Send reply error:" << ip;
    return;
  }
  if (socket_write(socket_fd, status_id_str.c_str(), 10) != 0) {
    LOG(ERROR) << "Send status_id error";
    return;
  }
  LOG(INFO) << "Submit Standard Test completed." << ip;
}

