#include "addfiletocontestprocessimp.h"

#include <vector>
#include <string>

#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
#include "data/datainterface.h"
#include "util/calulate.h"
#include "object/file.h"
#include "object/contest.h"
#include "object/list.h"
#include "object/bufsize.h"
#include "object/info.h"
#include "util/filetype.h"
#include "object/user.h"
using namespace std;

DECLARE_FLAGS(string, root_dir);

void toUpCase(string& suffix) {
  for (int i =0 ;i < suffix.length(); i++) {
    if (suffix[i] >= 'a' && suffix[i] <='z') {
      suffix[i] = 'A' + suffix[i] - 'a';
    }
  }
}

int getSuffix(string& suffix) {
  toUpCase(suffix);
  if (suffix == "JPG" || suffix == "JPEG" || suffix == "GIF"|| 
      suffix == "BMP" || suffix == "PNG") {
    suffix = "imp";
    return IMG;
  } else if (suffix == "cc" || suffix == "c" || suffix == "cpp") {
    suffix = "spj";
    return SPJ;
  } 
  return 0;
}

void AddFileToContestProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process add file to contest for:" << ip;
  BufSize buf;
  buf.alloc(length + 1);
  if (socket_read(socket_fd, buf.getBuf(), length) != length) {
    LOG(ERROR) << "Cannot read filename from:" << ip;
    return;
  }
  string filename(buf.getBuf(), buf.getBuf() + length);
  buf.alloc(11);
  if (socket_read(socket_fd, buf.getBuf(), 10) != 10) {
    LOG(ERROR) << "Cannot read contest_id from:" << ip;
    return;
  }
  Contest contest(atoi(buf.getBuf()));
  contest = DataInterface::getInstance().getContest(contest.getContestId());
  string::size_type pos = filename.find_last_of(".");
  string suffix("NULL");
  if (pos != string::npos) {
    suffix = filename.substr(pos+1);
  }
  LOG(INFO) << "get " << suffix << " file from:" << ip;
  buf.alloc(11);
  if (socket_read(socket_fd, buf.getBuf(), 10) != 10) {
    LOG(ERROR) << "Cannot read filelength from:" << ip;
    return;
  }
  int len = atoi(buf.getBuf());
  buf.alloc(len);
  if (socket_read(socket_fd, buf.getBuf(), len) != len) {
    LOG(ERROR) << "Cannot read file data from:" << ip;
    return;
  }
  int type = getSuffix(suffix);
  if (!type) {
    string url_length = "0000000000";
    socket_write(socket_fd, url_length.c_str(), 10);
    LOG(INFO) << "Unknown suffix from:" << ip;
    return;
  }
  string time = getLocalTimeAsString("%Y-%m-%d %H:%M:%S"); 
  string path = FLAGS_root_dir + "/file/" + suffix + "/c"+ 
                stringPrintf("%d", contest.getContestId()) + "/" + 
                time + "_" +
                filename; 
  DataInterface::getInstance().addFile(path, buf.getBuf(), len);
  File file;
  file.setPath(path);
  file.setType(type);
  DataInterface::getInstance().addFilePathtoContest(file,contest);
  string url = path;
  string url_len = stringPrintf("%010d", url.length());
  if (socket_write(socket_fd, url_len.c_str(), 10)) {
    LOG(ERROR) << "Cannot write url_len to:" << ip;
    return;
  }
  if (socket_write(socket_fd, url.c_str(), url.length())) {
    LOG(ERROR) << "Cannot write url to:" << ip;
    return;
  }
  LOG(INFO) << "Process add file to contest completed for:" << ip;
}

