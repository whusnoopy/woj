#include "judgethread.h"

#include <string>
#include <vector>
#include <set>

#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#include "data/datainterface.h"
#include "object/objectinc.h"
#include "judgecontrol/judgecontrol.h"
#include "util/calulate.h"
#include "base/judge_result.h"
#include "object/configure.h"
#include "base/logging.h"
#include "exception/pipeexception.h"

using namespace std;

const int MAX_PATH_LENGTH = 256;

bool JudgeThread::check(const string& ip) {
  return Configure::getInstance().getJudgeControlIpTabs().count(ip) > 0;
}

int JudgeThread::sendFile(int connect_fd, const JudgeMission& mission, const string& ip) {
  Status status = mission.status;
  //1.send header
  LOG(DEBUG) << "send header";
  char header[11];
  BufSize buf;
  memset(header, 0, sizeof(header));
  header[0] = mission.status.getLanguage();
  LOG(DEBUG) << "language :" << mission.status.getLanguage();
  *(unsigned int*) (header + 1) = htonl((unsigned int)(mission.status.getProblemId()));
  *(unsigned int*) (header + 5) = htonl((unsigned int)(mission.version));
  LOG(DEBUG) << stringPrintf("%018X", header);
  if (socket_write(connect_fd, header, 9) != 0) {
    LOG(SYS_ERROR) << "Cannot send header to :" << ip;
    JudgeControl::getInstance().addMission(mission);
    close(connect_fd);
    return -1;
  }

  //2.getReady
  char reply = '0';
  if (socket_read(connect_fd, &reply, 1) != 1) {
    LOG(SYS_ERROR) << "Cannot read header reply from:" << ip;
    JudgeControl::getInstance().addMission(mission);
    close(connect_fd);
    return -1;
  }
  if (reply != READY) {
    LOG(ERROR) << " Judge[" << ip << "]Cannot process the mission";
    if (reply == SYSTEM_ERROR) {
      status.setResult(static_cast<int>(reply));
      DataInterface::getInstance().updateStatus(status);
      JudgeControl::getInstance().putReJudgeItem(status);
    }
    else if (reply == UNSUPPORTED_SOURCE_FILE_TYPE) {
      JudgeControl::getInstance().addMission(mission);
    }
    return -2;
  }

  //3.send source length
  LOG(DEBUG) << "send source length";
  char len[4];
  LOG(DEBUG) << "code length" << status.getCodeLength();
  *(unsigned int*)(len) = htonl((unsigned int)status.getCodeLength());
  LOG(DEBUG) << stringPrintf("code length: %d(%04X)", status.getCodeLength(), *(unsigned int*)(len));
  if (socket_write(connect_fd, len, 4) != 0) {
    LOG(SYS_ERROR) << "Cannot send source length to :" << ip;
    JudgeControl::getInstance().addMission(mission);
    close(connect_fd);
    return -1;
  }

  //4.getReady
  if (socket_read(connect_fd, &reply, 1) != 1) {
    LOG(SYS_ERROR) << "Cannot read code length reply from:" << ip;
    JudgeControl::getInstance().addMission(mission);
    close(connect_fd);
    return -1;
  }
  if (reply != READY) {
    LOG(ERROR) << ip <<" Judge Cannot process the mission";
    status.setResult(SYSTEM_ERROR);
    DataInterface::getInstance().updateStatus(status);
    JudgeControl::getInstance().putReJudgeItem(status);
    return -2;
  }

  //5.send code 
  LOG(DEBUG) << "send code";
  if (socket_write(connect_fd, 
                   mission.source.c_str(), 
                   mission.status.getCodeLength()) != 0) {
    LOG(SYS_ERROR) << "Cannot send source to the judge:" << ip;
    JudgeControl::getInstance().addMission(mission);
    close(connect_fd);
    return -1;
  }
  
  //6.getReady
  if (socket_read(connect_fd, &reply, 1) != 1) {
    LOG(SYS_ERROR) << "Cannot read reply from:" << ip;
    JudgeControl::getInstance().addMission(mission);
    close(connect_fd);
    return -1;
  }
  if (reply == READY) {
    //7.send data length;
    LOG(DEBUG) << "send data length";
    string zip_dir = mission.data_path; 
    string zip_name = zip_dir + stringPrintf("%d.tar.gz", mission.version);
    if (access(zip_name.c_str(), R_OK) < 0) {
      string command = "tar -czf " + zip_name + " ";
      map<string, string>::const_iterator iter = mission.in_and_out_path.begin();
      while (iter != mission.in_and_out_path.end()) {
        command += iter->first.substr(iter->first.find_last_of("/") + 1) + " " + 
                   iter->second.substr(iter->second.find_last_of("/") + 1) + " ";
        iter++;
      }
      if (mission.spj)
        command += mission.spj_source_path.substr(mission.spj_source_path.find_last_of("/") + 1);
      LOG(DEBUG) << command;
      char path[MAX_PATH_LENGTH + 1];
      if (getcwd(path, sizeof(path)) == NULL) {
        LOG(SYS_ERROR) << "Cannot get pwd.";                
        JudgeControl::getInstance().addMission(mission);
        close(connect_fd);
        return -1;
      }

      if (chdir(zip_dir.c_str()) < 0) {
        LOG(SYS_ERROR) << "Cannot get change dir to problem.";                
        JudgeControl::getInstance().addMission(mission);
        close(connect_fd);
        return -1;
      }
      system(command.c_str());
      
      if (chdir(path) < 0) {
        LOG(SYS_ERROR) << "Cannot get change dir back.";                
        JudgeControl::getInstance().addMission(mission);
        close(connect_fd);
        return -1;
      }
    }
    ssize_t data_length = DataInterface::getInstance().getFileSize(zip_name);
    LOG(DEBUG) << stringPrintf("data_length: %d", data_length);
    char data_len[4];
    *(unsigned int*)(data_len) = htonl(data_length);
    if (socket_write(connect_fd, data_len, 4) != 0) {
      LOG(SYS_ERROR) << "Cannot send data length to:" << ip;
      JudgeControl::getInstance().addMission(mission);
      close(connect_fd);
      return -1;
    }

    //8.getReady
    if (socket_read(connect_fd, &reply, 1) != 1) {
      LOG(SYS_ERROR) << "Cannot read data length reply from:" << ip;
      JudgeControl::getInstance().addMission(mission);
      close(connect_fd);
      return -1;
    }
    if (reply != READY) {
      LOG(ERROR) << ip <<" Judge Cannot process the mission";
      status.setResult(SYSTEM_ERROR);
      DataInterface::getInstance().updateStatus(status);
      JudgeControl::getInstance().putReJudgeItem(status);
      return -2;
    }
    //9.send data;
    LOG(DEBUG) << "send data";
    FileData file_data = DataInterface::getInstance().getFile(zip_name);
    if (socket_write(connect_fd, file_data.buf, data_length) != 0) {
      LOG(SYS_ERROR) << "Cannot send zip data to :" << ip;
      JudgeControl::getInstance().addMission(mission);
      close(connect_fd);
      return -1;
    }
  } else if (reply == DATA_EXSIST) {
    
  }else if (reply == SYSTEM_ERROR) {
    LOG(INFO) << "System error when get code reply:" << ip;
    status.setResult(SYSTEM_ERROR) ;
    DataInterface::getInstance().updateStatus(status);
    JudgeControl::getInstance().putReJudgeItem(status);
    return -2;
  }else {
  
  }
 
  //10.getReady
  if (socket_read(connect_fd, &reply, 1) != 1) {
    LOG(SYS_ERROR) << "Cannot read data reply from:" << ip;
    JudgeControl::getInstance().addMission(mission);
    close(connect_fd);
    return -1;
  }
  if (reply != READY) {
    LOG(ERROR) << ip <<" Judge Cannot process the mission";
    status.setResult(SYSTEM_ERROR);
    DataInterface::getInstance().updateStatus(status);
    JudgeControl::getInstance().putReJudgeItem(status);
    return -2;
  }

  //11.send limit
  LOG(DEBUG) << "send limit";
  char limit[13];
  limit[0] = mission.in_and_out_path.size();
  *(unsigned int*)(limit + 1) = htonl((unsigned int)(mission.time_limit));
  *(unsigned int*)(limit + 5) = htonl((unsigned int)(mission.case_time_limit));
  *(unsigned int*)(limit + 9) = htonl((unsigned int)(mission.memory_limit));
  if (socket_write(connect_fd, limit, 13) != 0) {
    LOG(SYS_ERROR) << "Cannot send limit to :" << ip;
    JudgeControl::getInstance().addMission(mission);
    close(connect_fd);
    return -1;
  }
  
  //12.getReady
  if (socket_read(connect_fd, &reply, 1) != 1) {
    LOG(SYS_ERROR) << "Cannot read reply from:" << ip;
    JudgeControl::getInstance().addMission(mission);
    close(connect_fd);
    return -1;
  }
  if (reply == SYSTEM_ERROR) {
    LOG(INFO) << "System error when get limit reply:" << ip;
    status.setResult(SYSTEM_ERROR) ;
    DataInterface::getInstance().updateStatus(status);
    JudgeControl::getInstance().putReJudgeItem(status);
    return -2;
  }

  //13.compile
  LOG(DEBUG) << "compile";
  if (socket_read(connect_fd, &reply, 1) != 1) {
    LOG(SYS_ERROR) << "Cannot read reply from:" << ip;
    JudgeControl::getInstance().addMission(mission);
    close(connect_fd);
    return -1;
  }
  LOG(DEBUG) << stringPrintf("%d", (int)reply);
  if (reply != COMPILING) {
    LOG(ERROR) << "Process sequent error, should compile now:" << ip;
    status.setResult(SYSTEM_ERROR) ;
    DataInterface::getInstance().updateStatus(status);
    JudgeControl::getInstance().putReJudgeItem(status);
    return -2;
  }
  status.setResult(COMPILING);
  DataInterface::getInstance().updateStatus(status);

  //14.compile result
  LOG(DEBUG) << "get Complie reuslt from:" << connect_fd;
  if (socket_read(connect_fd, &reply, 1) != 1) {
    LOG(SYS_ERROR) << "Cannot read reply from:" << ip;
    JudgeControl::getInstance().addMission(mission);
    close(connect_fd);
    return -1;
  }
  LOG(DEBUG) << (int) reply;
  if (reply == COMPILE_ERROR) {
    LOG(INFO) << "Misson compile error:" << ip;
    status.setResult(COMPILE_ERROR);
    //15.CE information length 
    LOG(DEBUG) << "get Ce information length";
    uint16_t ce_length;
    char ce_len[2];
    if (socket_read(connect_fd, (char *)&ce_len, 2) != 2){
      LOG(SYS_ERROR) << "Cannot read reply from:" << ip;
      JudgeControl::getInstance().addMission(mission);
      close(connect_fd);
      return -1;
    }
    ce_length = ntohs(*(uint16_t*)ce_len);
    buf.alloc(ce_length);
    //16.get CE infromation
    LOG(DEBUG) << "get CE information";
    if (socket_read(connect_fd, buf.getBuf(), ce_length) != ce_length) {
      LOG(SYS_ERROR) << "Cannot read reply from:" << ip;
      JudgeControl::getInstance().addMission(mission);
      close(connect_fd);
      return -1;
    }
    string ce_info(buf.getBuf(), buf.getBuf() + ce_length);
    int ce_error_id = DataInterface::getInstance().addError(Error(0, ce_info));
    status.setErrorId(ce_error_id);
    DataInterface::getInstance().updateStatus(status);
    JudgeControl::getInstance().putReJudgeItem(status);
    return -2;
  }
  else if (reply == SYSTEM_ERROR) {
    LOG(ERROR) << "Compile system error now:" << ip;
    status.setResult(SYSTEM_ERROR) ;
    DataInterface::getInstance().updateStatus(status);
    JudgeControl::getInstance().putReJudgeItem(status);
    return -2;
  }
  for (int i = 0; i < mission.in_and_out_path.size(); i++) {
    //17.running
    LOG(DEBUG) << "running";
    if (socket_read(connect_fd, &reply, 1) != 1) {
      LOG(SYS_ERROR) << "Cannot read reply from:" << ip;
      JudgeControl::getInstance().addMission(mission);
      close(connect_fd);
      return -1;
    }
    LOG(DEBUG) << (int)reply;
    if (reply != RUNNING) {
      LOG(ERROR) << "Process sequent error, should run now:" << ip;
      status.setResult(SYSTEM_ERROR) ;
      DataInterface::getInstance().updateStatus(status);
      JudgeControl::getInstance().putReJudgeItem(status);
      return -2;
    }
    status.setResult(RUNNING);
    DataInterface::getInstance().updateStatus(status);
    //18.getReady
    if (socket_read(connect_fd, &reply, 1) != 1) {
      LOG(SYS_ERROR) << "Cannot read reply from:" << ip;
      JudgeControl::getInstance().addMission(mission);
      close(connect_fd);
      return -1;
    }
    bool is_break = true;
    switch(static_cast<int>(reply)) {
      case READY:
        is_break = false;
        break;
      case RUNTIME_ERROR_SIGSEGV:
      case RUNTIME_ERROR_SIGFPE:
      case RUNTIME_ERROR_SIGBUS:
      case RUNTIME_ERROR_SIGABRT:
        LOG(INFO) << "RE happened." << ip;
        break;
      case RUNTIME_ERROR_JAVA:
        LOG(INFO) << "RE JAVA/PAS happened." << ip;
        status.setResult(static_cast<int>(reply));
        //19.RE information length
        LOG(DEBUG) << "get Re information length";
        uint16_t re_length;
        char re_len[2];
        if (socket_read(connect_fd, (char *)&re_len, 2) != 2){
          LOG(SYS_ERROR) << "Cannot read reply from:" << ip;
          JudgeControl::getInstance().addMission(mission);
          close(connect_fd);
          return -1;
        }
        re_length = ntohs(*(uint16_t*)re_len);
        buf.alloc(re_length);
        //20.get RE infromation
        LOG(DEBUG) << "get Re infromation";
        if (socket_read(connect_fd, buf.getBuf(), re_length) != re_length) {
          LOG(SYS_ERROR) << "Cannot read reply from:" << ip;
          JudgeControl::getInstance().addMission(mission);
          close(connect_fd);
          return -1;
        }
        string re_info(buf.getBuf(), buf.getBuf() + re_length);
        int re_error_id = DataInterface::getInstance().addError(Error(0, re_info));
        status.setErrorId(re_error_id);
        DataInterface::getInstance().updateStatus(status);
        break;
    }
    if (is_break)
      break;
    //21.JUDGING
    LOG(DEBUG) << "judging";
    if (socket_read(connect_fd, &reply, 1) != 1) {
      LOG(SYS_ERROR) << "Cannot read reply from:" << ip;
      JudgeControl::getInstance().addMission(mission);
      close(connect_fd);
      return -1;
    }
    LOG(DEBUG) << (int)reply;
    if (reply != JUDGING) {
      LOG(ERROR) << "Process sequent error, should run now:" << ip;
      status.setResult(SYSTEM_ERROR) ;
      DataInterface::getInstance().updateStatus(status);
      JudgeControl::getInstance().putReJudgeItem(status);
      return -2;
    }
    status.setResult(JUDGING);
    DataInterface::getInstance().updateStatus(status);
    //22.getReady
    if (socket_read(connect_fd, &reply, 1) != 1) {
      LOG(SYS_ERROR) << "Cannot read reply from:" << ip;
      JudgeControl::getInstance().addMission(mission);
      close(connect_fd);
      return -1;
    }
    bool done = false;
    switch(static_cast<int>(reply)) {
      case ACCEPTED:
        LOG(INFO) << "AC:" << ip;
        status.setResult(ACCEPTED);
        break;
      case PRESENTATION_ERROR:
      case TIME_LIMIT_EXCEEDED:
      case MEMORY_LIMIT_EXCEEDED:
      case WRONG_ANSWER:
      case OUTPUT_LIMIT_EXCEEDED:
      case COMPILE_ERROR:
      case SYSTEM_ERROR:
        LOG(INFO) << "Not Ac :" << ip;
        done = true;
        break;
    }
    if (done) 
      break;
  }
  //23.end
  LOG(DEBUG) << "end";
  char ret[9];
  if (socket_read(connect_fd, ret, 9) != 9) {
    LOG(SYS_ERROR) << "Cannot read reply from:" << ip;
    JudgeControl::getInstance().addMission(mission);
    close(connect_fd);
    return -1;
  }
  status.setResult(static_cast<int>(ret[0]));
  status.setTime(ntohl(*(unsigned int*)(ret + 1)));
  status.setMemory(ntohl(*(unsigned int*)(ret + 5)));
  if (status.getType() == "S") {
    Problem problem = DataInterface::getInstance().getProblem(status.getProblemId());
    problem.setStandardTimeLimit(status.getTime());
    problem.setStandardMemoryLimit(status.getMemory());
    DataInterface::getInstance().updateProblemStandardLimit(problem);
  }
  DataInterface::getInstance().updateStatus(status);
  JudgeControl::getInstance().putReJudgeItem(status);
  if (static_cast<int>(ret[0]) == ACCEPTED) {
    DataInterface::getInstance().updateUserSolved(status, 1);
    DataInterface::getInstance().addProblemSolved(status.getProblemId(), 1);
    DataInterface::getInstance().addProblemUserSolved(status, 1);
  }
  LOG(DEBUG) << "Judge compelete.";
  return 0;
}

void JudgeThread::quit() {
  flag = true;
}

void JudgeThread::running() {
  int connect_fd;
  struct sockaddr_in child_addr;
  socklen_t len = sizeof(struct sockaddr);
  while (!flag) {
    pthread_mutex_lock(&JudgeControl::getInstance().socket_lock);
    connect_fd = accept(JudgeControl::getInstance().listen_fd_,
                        (struct sockaddr *)&child_addr, &len);
    pthread_mutex_unlock(&JudgeControl::getInstance().socket_lock);
    try {
      string ip = getIp(ntohl(child_addr.sin_addr.s_addr));
      if (!check(ip)) {
        LOG(ERROR) << "Unknown connection from:" << ip;
        close(connect_fd);
        continue;
      }
      LOG(INFO) << "Judge Control Connection from:" << ip;
      
      while (!flag) {
        JudgeControl::getInstance().semaphore.p();
        JudgeMission mission = JudgeControl::getInstance().getMission();
        int ret = sendFile(connect_fd, mission, ip);
        if (ret == -1) 
          break;
        LOG(DEBUG) << "ret:" << ret;
      }
    } catch (PipeException e) {
      close(connect_fd);
    }
  }
}

