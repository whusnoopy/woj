#ifndef _FLOOD_SERVER_DATA_CACHEMANGER_H__
#define _FLOOD_SERVER_DATA_CACHEMANGER_H__

#include <string>

#include "list.h"
#include "info.h"
#include "cache.h"
#include "contest.h"
using namespace std;

class CacheManager{
public:
  CacheManger() {}
  
  ContestStatitics getContestStatics(int contestId);
  ContestRankList getContestRankList(int contestId);
  StatusList getStatus();
  int updateStatus(const Status&);
  int updateContestStatitics(const Status&);
  int updateContestRankList(const Status&);
  Contest getContest();
  FileData getFileData(const string& filename);
  
  static CacheManager& getInstance() {
    if (instance == NULL) {
    	instance = new CacheManger;
    }
    return *instance;
  }
  static void destory() {
    delete instance;
  }
  
private:
  Cache<int, ContestStatitics> contest_statitics_cache;
  Cache<int, ContestRankList> contest_ranklist_cache;
  Cache<int, Contest> contest_cache;
  Cache<string, FileData> filedata_cache;
  Cache<int, status> status_cache;
  static CacheManager* instance;
};


#endif /*CACHEMANGER_H_*/
