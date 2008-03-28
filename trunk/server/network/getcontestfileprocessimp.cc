#include "getcontestfileprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "../util/calulate.h"
#include "../object/contest.h"
#include "../object/file.h"
#include "../object/filedata.h"
#include "../object/list.h"
#include "../object/info.h"
#include "../object/user.h"
using namespace std;

void GetContestFileProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process Get Contest File for:" << ip;
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
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find contest_id from data for:" << ip;
    return;
  }
  int contest_id = atoi(iter->c_str());
  Contest contest(contest_id);
  FileList file_list;
  //file_list = DataInterface::getInstance().getContestFile(contest);
  FileList::iterator file_iter = file_list.begin();
  string len = stringPrintf("%010d", file_list.size());
  if (socket_write(socket_fd, len.c_str(), 10)) {
    LOG(ERROR) << "Cannot send file num to :" << ip;
    return;
  }
  file_iter = file_list.begin();
  while (file_iter != file_list.end()) {
    string filename_length = stringPrintf("%010d", file_iter->getPath().length());
    if (socket_write(socket_fd, filename_length.c_str(), 10)) {
      LOG(ERROR) << "Cannot send filename length to:" << ip;
      return;
    }
    if (socket_write(socket_fd, file_iter->getPath().c_str(), file_iter->getPath().length())) {
      LOG(ERROR) << "Cannot send filename to:" << ip;
      return;
    }
    int data_size = 0;
    //data_size = DataInterface::getInstance().getFileSize(file_iter->getPath());
    string datalen = stringPrintf("%010d", data_size);
    if (socket_write(socket_fd, datalen.c_str(), 10)) {
      LOG(ERROR) << "Cannot send data size to:" << ip;
      return;
    }
    FileData file_data;
    file_data.buf = NULL;
    //file_data = DataInterface::getInstance().getFile(file_iter->getPath());
    if (socket_write(socket_fd, file_data.buf, data_size)) {
      LOG(ERROR) << "Cannot send data to:" << ip;
      if (file_data.buf != NULL) {
        delete[] file_data.buf;
      }
      return;
    }
    if (file_data.buf != NULL) {
      delete[] file_data.buf;
    }
  }
  LOG(ERROR) << "Process Get Contest File completed for:" << ip;
}

