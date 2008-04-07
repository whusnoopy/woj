#ifndef _FLOOD_SERVER_DATA_CACHEMANGER_H__
#define _FLOOD_SERVER_DATA_CACHEMANGER_H__

#include <string>
#include <vector>

#include <pthread.h>

#include "data/cache.h"
#include "object/objectinc.h"
using namespace std;

class CacheManager{
public:
  CacheManager();
  ~CacheManager();

  ContestStatistics getContestStatistics(int contest_id);
  ContestRankList getContestRankList(int contest_id);
  StatusList getStatus();
  int updateStatus(const Status&);
  int updateContestStatistics(const Status&, bool);
  int updateContestRankList(const Status&, bool);
  Contest getContest(int contest_id);
  FileData getFileData(const string& filename);
  int addStatus(const Status& status);
  
  static CacheManager& getInstance() {
    if (instance == NULL) {
    	instance = new CacheManager;
    }
    return *instance;
  }
  static void destroy() {
    delete instance;
  }
  
private:
  Cache<int, ContestStatistics> contest_statistics_cache;
  Cache<int, ContestRankList> contest_ranklist_cache;
  Cache<int, Contest> contest_cache;
  Cache<string, FileData> filedata_cache;
  Cache<int, Status> status_cache;

  pthread_mutex_t contest_statistics_lock;
  pthread_mutex_t contest_ranklist_lock;
  pthread_mutex_t contest_lock;
  pthread_mutex_t status_lock;
  pthread_mutex_t filedata_lock;

  static CacheManager* instance;
};


#endif /*CACHEMANGER_H_*/
