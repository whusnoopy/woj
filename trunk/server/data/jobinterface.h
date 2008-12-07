// Copyright 2008 Flood Team of Wuhan Univ.
// Author : ooeyusea@gmail.com (Qian, LIU)
// Modified : yewen@mail.whu.edu.cn (Wen, YE)

#ifndef _FLOOD_SERVER_DATA_JOBINTERFACE_H__
#define _FLOOD_SERVER_DATA_JOBINTERFACE_H__

#include <string>

#include "server/object/job.h"
#include "server/object/list.h"

#include "server/data/connectionfactory.h"

using namespace std;

class JobInterface: public ConnectionFactory{
public:
  JobInterface() {
  }
  
  ~JobInterface() {
  }

  int addJob(const Job&);
  int addJobToClass(int job_id, const Class& mclass);
  int addProblemToJob(int job_id, const vector<int>& problem_list);
  int addSet(const vector<int>& problem_list, int num);
  int addSetToJob(int job_id, int set_id);
  int disableJob(int job_id, bool available);
  JobList getJobList(int course_id);
  JobList getJobList(const string& student);
  Job getJob(int job_id);
  int deleteSetForJob(int job_id, int set_id);
  int updateJob(const Job&);
  int updateSet(const SetItem&);

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
