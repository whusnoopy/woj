// Copyright 2008 Flood Team of Wuhan Univ.
// Author : ooeyusea@gmail.com (Qian, LIU)
// Modified : yewen@mail.whu.edu.cn (Wen, YE)

#include <vector>
#include <deque>
#include <map>
#include <string>
#include <iostream>

#include "base/logging.h"
#include "base/flags.h"

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

int main(int argc, char* argv[]) {
  // Parse flags or die
  if (parseFlags(argc, argv)) {
    LOG(SYS_ERROR) << "Cannot parse flags!";
    return -1;
  }

  // Init works
  init();

  Server server;
  server.initServer();
  server.start();

  ClientServer client_server;
  client_server.initClientServer();
  client_server.start();
  
  JudgeControl::getInstance().start();

  // Command console
  cout << "Server is start up now, congratulations!\n";
  string in;
  while (true) {
    cout << "  Input contral command: ";
    cin>>in;
    if (in == "reconfigure") {
      Configure::reGet();
      LOG(INFO) << "Reconfigured finish";
    } else if(in == "quit") { 
      LOG(INFO) << "Catched quit command, stop server now";
      break;
    } else if (in == "configure") {
      Configure::getInstance().output();
    } else {
      cout << "Unknown command: \"" << in << "\"\n";
      cout << "Usage: \n";
      cout << "  configure\n";
      cout << "  reconfigure\n";
      cout << "  quit\n";
    }
  }
  cout << "Command console has been stoped.\n";

  // Destroy works
  // TODO: Actually, Qian used a strange way to do the destroy work, the join
  // functions are blocking, so we cannot touch the DEBUG lines forever
  server.join();
  LOG(DEBUG) << "Touch this line";
  JudgeControl::getInstance().join();
  LOG(DEBUG) << "Touch this line";
  destroy();
  LOG(DEBUG) << "Touch this line";

  return 0;
}

