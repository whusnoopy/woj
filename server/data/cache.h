#ifndef _FLOOD_SERVER_DATA_CACHE_H__
#define _FLOOD_SERVER_DATA_CACHE_H__

#include <map>
#include <string>
#include <vector>

#include <time.h>
#include <pthread.h>
#include <errno.h>

#include "cacheentry.h"
#include "cachekey.h"
#include "comparequeue.h"
#include "object/inc.h"
#include "util/thread.h"
using namespace std;

template <typename Key, typename Value>
class Cache;

template <typename Key, typename Value>
class CacheCleaner : public Thread{
public:
  typedef CacheKey<Key> CacheKeyType;
  CacheCleaner(Cache<Key, Value>* father) { 
    father_ = father;
    quitflag = false; 
  }

	void running(){
		while (!quitflag){
      struct timespec ts;
      struct timespec tr;
      ts.tv_sec = 300;
      ts.tv_nsec = 0;
      while (1) {
        int nano_ret = nanosleep(&ts, &tr);
        if (nano_ret == 0) 
          break;
        else if (errno == EINTR) {
          ts.tv_sec -= tr.tv_sec;
          if (ts.tv_sec <= 0) 
            break;
          continue;
        } else {
          LOG(ERROR) << "sleep interrupted error.";
          break;
        }
      }
      pthread_mutex_lock(&father_->lock);
      LOG(DEBUG) << "cleaner lock:" << pthread_self();
  		int now = time(NULL);
      LOG(DEBUG) << "Begin cleaner the cache....";
      typename list<CacheKeyType>::iterator iter = father_->access_queue.begin();
      while (iter != father_->access_queue.end()){
        if (iter->getLastAccessTime() + iter->getTimeOut() > now){
          Key key_buf = iter->getKey();
          iter = father_->access_queue.erase(iter);
          LOG(INFO) << "Cleaner clean the cache:" << key_buf;
          if (father_->cache_map.count(key_buf) != 0) {
            father_->cache_map.erase(key_buf);
          }
          LOG(DEBUG) << "Cleaner clean up the cache:" << key_buf;
          continue;
        } else
          iter++;
      }
      LOG(DEBUG) << "End cleaner the cache.....";
      pthread_mutex_unlock(&father_->lock);
      LOG(DEBUG) << "cleaner unlock:" << pthread_self();
  	}
  }
  void quit() {
    quitflag = true;
  }
private:
  bool quitflag;
  Cache<Key, Value>* father_;
};


template <typename Key, typename Value>
class Cache{
public:
  friend class CacheCleaner<Key, Value>;
	typedef CacheKey<Key> CacheKeyType;
	typedef CacheEntry<CacheKey<Key>, Value> CacheEntryType;
	Cache(int time_out, int capacity) 
    : access_queue(compare),
      time_out_(time_out), 
      capacity_(capacity),
      cleaner(this){
    pthread_mutex_init(&lock, NULL);
    if (time_out != -1)
		  cleaner.start();
	}

  ~Cache() {
    LOG(INFO) << "Should not be seen";
    pthread_mutex_destroy(&lock);
  }
/*	
	map<Key, Value> getKeys() const {
	  return cache_map;
	}
*/
  vector<Value> getValues() {
    pthread_mutex_lock(&lock);
    LOG(DEBUG) << "get values lock";
    vector<Value> value_list;
    typename map<Key, CacheEntryType>::iterator iter = cache_map.begin(); 
    while (iter != cache_map.end()) {
      value_list.push_back(iter->second.getValue());
      iter++;
    }
    pthread_mutex_unlock(&lock);
    LOG(DEBUG) << "get values unlock";
    return value_list;
  }

	Value getValue(const Key& key) {
    pthread_mutex_lock(&lock);
    LOG(DEBUG) << "get value lock";
    Value value;
		if (cache_map.count(key) != 0) {
			CacheEntryType entry = cache_map[key];
			CacheKeyType cache_key = entry.getKey();
			access_queue.remove(cache_key);
			cache_key.setLastAccessTime(atoi(getLocalTimeAsString("%Y-%m-%d %H:%M:%S").c_str()));
			access_queue.add(cache_key);      
      pthread_mutex_unlock(&lock);
      LOG(DEBUG) << "get value unlock";
			return entry.getValue();
		} else{
      pthread_mutex_unlock(&lock);
      LOG(DEBUG) << "get value unlock";
			return value;
		}
	}
  bool empty() {
    pthread_mutex_lock(&lock);
    bool ret = cache_map.size() == 0;
    pthread_mutex_unlock(&lock);
    return ret;
  }

	void put(const Key& key, const Value& value, int time_out){
		if (capacity_ == 0 )
		  return;
    pthread_mutex_lock(&lock);
    LOG(DEBUG) << "put lock";
		CacheKeyType cache_key(key, time(NULL), time_out);
		CacheEntryType entry(cache_key, value);
		if (cache_map.count(key) == 0) {
			if (cache_map.size() >= capacity_){
				CacheKeyType remove_key = access_queue.removeFirst();
				cache_map.erase(remove_key.getKey());
			}
		}else {
		  access_queue.remove(cache_map[key].getKey());
		}
		cache_map[key] = entry;
		access_queue.add(cache_key);
    pthread_mutex_unlock(&lock);
    LOG(DEBUG) << "put unlock";
	}
	
	void put(const Key& key, const Value& value){
		put(key, value, time_out_);
	}
	void remove(const Key& key){
    pthread_mutex_lock(&lock);
    LOG(DEBUG) << "remove lock";
		if (cache_map.count(key) != 0) {
			access_queue.remove(cache_map[key].getKey());
			cache_map[key].erase(key);
		}
    pthread_mutex_unlock(&lock);
    LOG(DEBUG) << "remove unlock";
	}
	bool has(const Key& key){
    pthread_mutex_lock(&lock);
    LOG(DEBUG) << "has lock";
    bool ret = cache_map.count(key) != 0;
    pthread_mutex_unlock(&lock);
    LOG(DEBUG) << "has unlock";
		return ret;
	}
	Value removeKey(const Key& key){
    pthread_mutex_lock(&lock);
    LOG(DEBUG) << "removekey lock";
    Value value;
		if (cache_map.count(key) != 0) {
			value = cache_map[key].getValue();
			access_queue.remove(cache_map[key].getKey());
			cache_map.erase(key);
      pthread_mutex_unlock(&lock);
      LOG(DEBUG) << "removekey unlock";
			return value;
		} 
    pthread_mutex_unlock(&lock);
    LOG(DEBUG) << "removekey unlock";
		return value;
	}
  
  static int compare(const CacheKeyType& a, const CacheKeyType& b) {
  	return (a.getLastAccessTime() + a.getTimeOut()) 
  	       - (b.getLastAccessTime() + b.getTimeOut());
  }
  
  void join() {
  	cleaner.join();
  }

private:
  map<Key, CacheEntryType> cache_map;
  CompareQueue<CacheKeyType> access_queue;
  int time_out_;
  int capacity_;
  CacheCleaner<Key, Value> cleaner;
  pthread_mutex_t lock;
};

#endif
