#include "discusslistprocessimp.h"

#include <string>
#include <vector>
#include <set>

#include "../object/discuss.h"
#include "../object/info.h"
#include "../object/list.h"
#include "../util/calulate.h"
#include "base/util.h"
#include "base/logging.h"
#include "base/flags.h"
using namespace std;

string prefixPlus(int level) {
  string ret;
  for (int i = 0; i < level; i++) {
    ret += "+";
  }
  return ret;
}

void findTree(int discuss_id, string& buf, int level) {
  DiscussList discuss_list;
  //discuss_list = DataInterface::getInstance().getReplyDiscussList(discuss_id);
  DiscussList::iterator iter = discuss_list.begin();
  while (iter != discuss_list.end()) {
    buf += prefixPlus(level) + stringPrintf("\001%d\001%s\001%s\001%s\001%d\001%d",
                                            iter->discuss_id,
                                            iter->title.c_str(),
                                            iter->date.c_str(),
                                            iter->user_id.c_str(),
                                            iter->problem_id,
                                            iter->contest_id);
    findTree(iter->discuss_id, buf, level+1);
    iter++; 
  }
}

void DiscussListProcessImp::process(int socket_fd, const string& ip, int length){
  LOG(INFO) << "Process Discuss List for:" << ip;
  char* buf;
  buf = new char[length+1];
  memset(buf,0,sizeof(buf));
  if (socket_read(socket_fd, buf, length) != length) {
    LOG(ERROR) << "Cannot read data from:" << ip;
    delete[] buf;
    return;
  }
  string read_data(buf);
  delete[] buf;
  vector<string> datalist;
  spriteString(read_data, 1, datalist);
  
  DiscussInfo discuss_info;
  bool search = false;
  vector<string>::iterator iter = datalist.begin();
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find title from data for:" << ip;
    return;
  }
  if (*iter != "?"){
    discuss_info.title = *iter;
    search = true;
  }else 
    discuss_info.title = "NULL";
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find problem_id from data for:" << ip;
    return;
  }
  discuss_info.problem_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find contest_id from data for:" << ip;
    return;
  }
  discuss_info.contest_id = atoi(iter->c_str());
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find user_id from data for:" << ip;
    return;
  }
  if (*iter != "?") {
    discuss_info.user_id = *iter;
    search = true;
  }else 
    discuss_info.user_id = "NULL";
  iter++;
  if (iter == datalist.end()) {
    LOG(ERROR) << "Cannot find page_id from data for:" << ip;
    return;
  }
  discuss_info.page_id = atoi(iter->c_str());
  iter++;

  string databuf;
  TopicSet topic_set;
  if (search) {
    DiscussList discuss_list;
    //discuss_list = DataInterface::getInstance().getDiscussList(discuss_info);
    bool first = true;
    DiscussList::iterator discuss_iter = discuss_list.begin();
    while (discuss_iter != discuss_list.end()) {
      if (first) {
        databuf += stringPrintf("+\001%d\001%s\001%s\001%s\001%d\001%d",
                                discuss_iter->discuss_id,
                                discuss_iter->title.c_str(),
                                discuss_iter->date.c_str(),
                                discuss_iter->user_id.c_str(),
                                discuss_iter->problem_id,
                                discuss_iter->contest_id);
        first = false;
      }else {
        databuf += stringPrintf("\001+\001%d\001%s\001%s\001%s\001%d\001%d",
                                discuss_iter->discuss_id,
                                discuss_iter->title.c_str(),
                                discuss_iter->date.c_str(),
                                discuss_iter->user_id.c_str(),
                                discuss_iter->problem_id,
                                discuss_iter->contest_id);
      }
      discuss_iter++;
    }
  }else {
    //topic_set = DataInterface::getInstance().getDiscussTopicSet(discuss_info);
    Discuss discuss;
    TopicSet::iterator set_iter = topic_set.begin();
    bool first = true;
    while (set_iter != topic_set.end()) {
      //discuss = DataInterface::getInstance().getDiscuss(*set_iter);
      if (first) {
        databuf += stringPrintf("+\001%d\001%s\001%s\001%s\001%d\001%d",
                                discuss.getMessageId(),
                                discuss.getTitle().c_str(),
                                discuss.getDate().c_str(),
                                discuss.getUserId().c_str(),
                                discuss.getProblemId(),
                                discuss.getContestId()); 
        first = false;
      }else {
        databuf += stringPrintf("\001+\001%d\001%s\001%s\001%s\001%d\001%d",
                                discuss.getMessageId(),
                                discuss.getTitle().c_str(),
                                discuss.getDate().c_str(),
                                discuss.getUserId().c_str(),
                                discuss.getProblemId(),
                                discuss.getContestId());
      }
      findTree(*set_iter, databuf, 2);
      set_iter++;
    }
  }

  string len = stringPrintf("%010d",databuf.length());
  if (socket_write(socket_fd, len.c_str(), 10)){
    LOG(ERROR) << "Send data failed to:" << ip;
    return;
  }
  if (socket_write(socket_fd, databuf.c_str(), databuf.length())) {
    LOG(ERROR) << "Cannot return data to:" << ip;
    return;
  }
  LOG(INFO) << "Process DiscussList completed for" << ip;
}

