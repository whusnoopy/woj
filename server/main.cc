#include <vector>
#include <deque>
#include <map>
#include <string>
#include <iostream>

#include "judgecontrol/judgecontrol.h"
#include "data/datainterface.h"
#include "network/server.h"
#include "object/configure.h"
#include "data/databaseinterface.h"
#include "data/fileinterface.h"
#include "data/cachemanager.h"

using namespace std;

void init() {
  Configure::getInstance();
  DataInterface::getInstance();
  FileInterface::getInstance();
  DatabaseInterface::getInstance();
  CacheManager::getInstance();
  JudgeControl::getInstance().initJudge();
}


void destroy() {
  Configure::destroy();
  DataInterface::destroy();
  FileInterface::destroy();
  DatabaseInterface::destroy();
  CacheManager::destroy();
  JudgeControl::destroy();
}

int main(int argc, char* argv[]){
  if (parseFlags(argc,argv)) {
    LOG(SYS_ERROR) << "Cannot parse flags!";
    return -1;
  }
  init();
  Server server;
  server.initServer();
  server.start();
  JudgeControl::getInstance().start();
  JudgeControl::getInstance().join();
  server.join();
  destroy();
  return 0;
}

