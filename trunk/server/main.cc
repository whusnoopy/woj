#include <vector>
#include <deque>
#include <map>
#include <string>
#include <iostream>

#include "judgecontrol/judgecontrol.h"
#include "data/datainterface.h"
#include "network/server.h"
#include "network/clientserver.h"
#include "object/configure.h"
#include "data/databaseinterface.h"
#include "data/fileinterface.h"
#include "data/cachemanager.h"
#include "exception/signaltranslator.h"
#include "exception/pipeexception.h"

#include "base/logging.h"
#include "base/flags.h"

using namespace std;

DECLARE_FLAGS(string, configure_path);

void init() {
  Configure::init();
  Configure::getInstance();
  DataInterface::getInstance();
  FileInterface::getInstance();
  DatabaseInterface::getInstance();
  CacheManager::getInstance();
  JudgeControl::getInstance().initJudge();
  SignalTranslator<PipeException>::getInstance();
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
  string in;
  init();
  Server server;
  server.initServer();
  ClientServer client_server;
  client_server.initClientServer();
  server.start();
  JudgeControl::getInstance().start();
  client_server.start();
  while (1) {
    cin>>in;
    if (in == "reconfigure") {
      Configure::reGet();
      LOG(INFO) << "ReConfigure.";
    } else if(in == "quit") 
      break;
    else if (in == "configure") {
      Configure::getInstance().output();
    }
    else {
      cout << "Unknown command:" << in;
      cout << "please tape reconfigure, quit";
    }
  }
  JudgeControl::getInstance().join();
  server.join();
  destroy();
  return 0;
}

