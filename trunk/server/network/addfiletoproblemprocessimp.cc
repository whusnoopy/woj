#include <vector>
#include <string>

#include "base/flags.h"
#include "base/logging.h"

#include "object/inc.h"
#include "data/datainterface.h"
#include "util/calulate.h"
#include "object/file.h"
#include "object/problem.h"
#include "object/list.h"
#include "object/bufsize.h"
#include "object/info.h"
#include "object/user.h"
#include "util/filetype.h"
#include "addfiletoproblemprocessimp.h"

using namespace std;

DECLARE_FLAGS(string, root_dir);

void upCase(string& suffix) {
  for (int i =0 ;i < suffix.length(); i++) {
    if (suffix[i] >= 'a' && suffix[i] <='z') {
      suffix[i] = 'A' + suffix[i] - 'a';
    }
  }
}

int findSuffix(string& suffix) {
  upCase(suffix);
  if (suffix == "JPG" || suffix == "JPEG" || suffix == "GIF"|| 
      suffix == "BMP" || suffix == "PNG") {
    suffix = "imp";
    return IMG;
  } else if (suffix == "SPJ") {
    suffix = "spj";
    return SPJ;
  } else if (suffix == "CC" || suffix == "C" || suffix == "PAS" || suffix == "JAVA") {
    return STANDARD_SOURCE;
  }
  return 0;
}

void AddFileToProblemProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process add file to problem for:" << ip;
  BufSize buf;
  buf.alloc(length + 1);
  if (socket_read(socket_fd, buf.getBuf(), length) != length) {
    LOG(ERROR) << "Cannot read filename from:" << ip;
    return;
  }
  string filename(buf.getBuf(), buf.getBuf() + length);
  buf.alloc(11);
  if (socket_read(socket_fd, buf.getBuf(), 10) != 10) {
    LOG(ERROR) << "Cannot read problem_id from:" << ip;
    return;
  }
  Problem problem(atoi(buf.getBuf()));
  problem = DataInterface::getInstance().getProblem(problem.getProblemId());
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
    LOG(ERROR) << "Canot read file data from:" << ip;
    return;
  }
  int type = findSuffix(suffix);
  if (!type) {
    string url_length = "0000000000";
    socket_write(socket_fd, url_length.c_str(), 10);
    LOG(INFO) << "Unknown suffix from:" << ip;
    return;
  }
  string time = getLocalTimeAsString("%Y-%m-%d %H:%M:%S"); 
  string path;
  if (type == IMG)
    path = FLAGS_root_dir + "/file/" + suffix + "/p" + 
           stringPrintf("%d", problem.getProblemId()) +
           "/" + filename; 
  else if (type == SPJ) 
    path = getProblemDataPath(problem.getProblemId()) + string("spj.cc");
  else if (type == STANDARD_SOURCE) 
    path = getProblemDataPath(problem.getProblemId()) + string("stc.cc");
  DataInterface::getInstance().addFile(path, buf.getBuf(), len);
  File file;
  file.setPath(path);
  file.setType(type);
  int ret = DataInterface::getInstance().addFilePathtoProblem(file,problem);
  if (ret < 0) {
    sendReply(socket_fd, 'N');
    LOG(INFO) << "Add File Error:" << ip;
    return;
  }
  if (sendReply(socket_fd, 'Y') != 0) {
    LOG(ERROR) << "Cannot send reply to:" << ip;
    return;
  }
  LOG(INFO) << "Process add file to problem completed for:" << ip;
}

