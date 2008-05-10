#ifndef _FLOOD_SERVER_DATA_JOBINTERFACE_H__
#define _FLOOD_SERVER_DATA_JOBINTERFACE_H__

#include <string>

#include "object/job.h"
#include "object/list.h"
#include "data/connectionfactory.h"
using namespace std;

class JobInterface: public ConnectionFactory{
public:
  JobInterface() {
  }
  
  ~JobInterface() {
  }

  static JobInterface& getInstance(){
    if(instance == NULL)
      instance = new JobInterface;
    return *instance;
  }

  static void destroy(){
    if(instance)
      delete instance;
    instance = NULL;
  }
private:
  static JobInterface * instance;
  
};


#endif
