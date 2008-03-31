#include "cachemanager.h"

#include "data/databaseinterface.h"
#include "data/fileinterface.h"

CacheManager* CacheManager::instance = NULL;

CacheManager::CacheManager()
  : contest_statistics_cache(300000,10),
    contest_ranklist_cache(300000,10),
    contest_cache(300000,20),
    filedata_cache(300000,50),
    status_cache(-1,25){

}

ContestStatistics CacheManager::getContestStatistics(int contest_id) {
  if (!contest_statistics_cache.has(contest_id)) {
    contest_statistics_cache.put(contest_id, 
                                 DatabaseInterface::getInstance().getContestStatistics(contest_id));
  }
  return contest_statistics_cache.getValue(contest_id);
}

ContestRankList CacheManager::getContestRankList(int contest_id) {
  if (!contest_ranklist_cache.has(contest_id)) {
    contest_ranklist_cache.put(contest_id,
                               DatabaseInterface::getInstance().getContestRankList(contest_id));
  }
  return contest_ranklist_cache.getValue(contest_id);
}

StatusList CacheManager::getStatus() {
  if (status_cache.empty()) {
    StatusList status_list = DatabaseInterface::getInstance().getNoSearchStatus();
    StatusList::iterator iter = status_list.begin();
    while (iter != status_list.end()) {
      status_cache.put(iter->getStatusId(), *iter);
    }
  }
  return status_cache.getValues();
}

int CacheManager::updateStatus(const Status& status) {
  status_cache.put(status.getStatusId(), status); 
  return 1;
}

int CacheManager::updateContestStatistics(const Status& status, bool add) {
  int contest_id = status.getContestId();
  ContestStatistics contest_statistics = getContestStatistics(contest_id);
  if (!add) {
    ContestAcBefore contest_acbefore = {status.getUserId(), 
                                        status.getSubmitTime(),
                                        status.getContestId(),
                                        status.getProblemId()};
    bool ac = DatabaseInterface::getInstance().checkContestAcBefore(contest_acbefore);
    if (!ac) {
      ContestStatistics::iterator iter = contest_statistics.begin();
      while (iter != contest_statistics.end()) {
        if (iter->problem_id == status.getProblemId()) {
          break;
        }
        iter++;
      }
      iter->Total++;
  	  switch (status.getLanguage()) {
  	    case 0:
  	    case 1:
  	      iter->C_CPP++;
  	      break;
  	    case 2:
  	      iter->Java++;
  	      break;
  	    case 3:
  	      iter->Pascal++;
  	  }
  	  switch (status.getResult()) {
  		 	case ACCEPTED:
  		    iter->AC++;
  	      break;
  		  case PRESENTATION_ERROR:
  		    iter->PE++;
  	      break;
  		  case TIME_LIMIT_EXCEEDED:
  		    iter->TLE++;
  	      break;
  		  case MEMORY_LIMIT_EXCEEDED:
  		    iter->MLE++;
  	      break;
  		  case WRONG_ANSWER:
  		    iter->WA++;
  	      break;
  		  case OUTPUT_LIMIT_EXCEEDED:
  		    iter->OLE++;
  	      break;
  		  case COMPILE_ERROR:
  		    iter->CE++;
  	      break;
  		  case RUNTIME_ERROR_SIGSEGV:
  		  case RUNTIME_ERROR_SIGFPE:
  		  case RUNTIME_ERROR_SIGBUS:
  		  case RUNTIME_ERROR_SIGABRT:
  		  case RUNTIME_ERROR_JAVA:
  		  case RUNTIME_ERROR_PAS:
  		    iter->RE++;
  	      break;
  	  }  
    }
  }else{
    //because I find that it is more convent only update the cache when 
    //the result is judge ;
  }
  contest_statistics_cache.put(contest_id, contest_statistics);
  return 1;
}

int CacheManager::updateContestRankList(const Status& status, bool add) {
  int contest_id = status.getContestId();
  ContestRankList contest_ranklist = getContestRankList(contest_id);
  if (!add) {
    ContestAcBefore contest_acbefore = {status.getUserId(), 
                                        status.getSubmitTime(),
                                        status.getContestId(),
                                        status.getProblemId()};
    int problem_id = status.getProblemId();
    int contest_id = status.getContestId();
    Contest contest = DatabaseInterface::getInstance().getContest(contest_id);
    string user_id = status.getUserId();
    bool ac = DatabaseInterface::getInstance().checkContestAcBefore(contest_acbefore);
    if (!ac) {
      ContestRankList::iterator iter = contest_ranklist.begin();
      while (iter != contest_ranklist.end()) {
        if (iter->user_id == status.getUserId()) {
          break;
        }
        iter++;
      }
      if (iter == contest_ranklist.end()) {
        ContestRankListItem item;
        item.accepted = 0;
        item.penalty = 0;
        item.user_id = user_id;
        iter = contest_ranklist.insert(iter, item);
      }
      ContestProblemTime time_item;
      if (iter->problem_penalty.count(problem_id) == 0){
        time_item.ac = false;
        time_item.penalty = 0 ;
        time_item.problem_id = problem_id;
        time_item.submit = 0;
        time_item.in_contest_id = DatabaseInterface::getInstance().getInContestId(contest_id, problem_id);
        iter->problem_penalty[problem_id]=time_item;
      }
      if (!iter->problem_penalty[problem_id].ac)
        iter->problem_penalty[problem_id].submit++;
      if (status.getResult() == ACCEPTED && !iter->problem_penalty[problem_id].ac){
        iter->accepted++;
        iter->problem_penalty[problem_id].ac = true;
        iter->problem_penalty[problem_id].penalty = caltime(status.getSubmitTime(), 
                                                            contest.getStartTime()) + 
          20*60*(iter->problem_penalty[problem_id].submit - 1);
        iter->penalty += iter->problem_penalty[problem_id].penalty;
        if (iter->problem_penalty[problem_id].in_contest_id == -1) {
          LOG(ERROR) << "Unknown Contest problem ";
          contest_ranklist.erase(iter);
          return 0;
        }
      }
    }
  }else {
  }
  contest_ranklist_cache.put(contest_id, contest_ranklist);
  return 1;
}

Contest CacheManager::getContest(int contest_id) {
  if (!contest_cache.has(contest_id)) {
    contest_cache.put(contest_id, 
                      DatabaseInterface::getInstance().getContest(contest_id));
  }
  return contest_cache.getValue(contest_id);
}

FileData CacheManager::getFileData(const string& filename) {
  if (!filedata_cache.has(filename)) {
    filedata_cache.put(filename, 
                       FileInterface::getInstance().getFile(filename)); 
  }
  return filedata_cache.getValue(filename);
}


