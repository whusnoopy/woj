#include "submitprocessimp.h"

#include <vector>
#include <string>

#include "object/objectinc.h"
#include "object/bufsize.h"
#include "util/calulate.h"
#include "data/datainterface.h"
#include "judgecontrol/judgecontrol.h"
using namespace std;

void SubmitProcessImp::process(int socket_fd, const string& ip, int length) {
  LOG(INFO) << "Process the Submit for:" << ip;
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
  string user_id, password, submit_ip;
  int problem_id = 0, contest_id = 0, language = 0, code_length = 0;
  bool share_code = false;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id from data for:" << ip;
    return;
  }
  user_id = *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find password from data for:" << ip;
    return;
  }
  password = *iter;
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find problem_id from data for:" << ip;
    return;
  }
  problem_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find contest_id from data for:" << ip;
    return;
  }
  contest_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find language from data for:" << ip;
    return;
  }
  language = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find code_length from data for:" << ip;
    return;
  }
  code_length = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find share_code from data for:" << ip;
    return;
  }
  problem_id = (*iter == "Y");
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find ip from data for:" << ip;
    return;
  }
  submit_ip = *iter;
  iter++;
  BufSize source_buf;
  source_buf.alloc(code_length);
  if (socket_read(socket_fd, source_buf.getBuf(), code_length) != code_length) {
    LOG(ERROR) << "Cannot read source from:" << ip;
    return;
  }
  string source(source_buf.getBuf(), source_buf.getBuf() + code_length);
  //check user;
  User user = DataInterface::getInstance().getUserInfo(user_id);
  if (user.getPassword() != password) {
    sendReply(socket_fd, 'N');
    return;
  }
  user.setSubmit(user.getSubmit() + 1);
  DataInterface::getInstance().updateUser(user);
  int code_id = DataInterface::getInstance().addCode(Code(0, share_code, source));
  status.setUseId(user_id);
  status.setProblemId(problem_id);
  status.setContestId(contest_id);
  status.setLanguage(language);
  status.setCodeLength(code_length);
  status.setSubmitIp(submit_ip);
  status.setSubmitTime(getLocalTimeAsString("%Y-%m-%d %H:%M:%S"));
  status.setResult(0);
  status.setMemory(-1);
  status.setTime(-1);
  status.setCodeId(code_id);
  status.setErrorId(0);
  int status_id = DataInterface::getInstance().addStatus(status);
  status.setStatusId(status_id);
  Problem problem = DataInterface::getInstance().getProblem(problem_id);
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
  mission.data_path = getProblemDataPath(problem_id);
  JudgeControl::getInstance().addMission(mission);
  if (sendReply(socket_fd, 'Y') != 0) {
    LOG(ERROR) << "Send reply error:" << ip;
    return;
  }
  LOG(INFO) << "Submit process completed." << ip;
}

