#ifndef _FLOOD_SERVER_DATA_CACHE_H__
#define _FLOOD_SERVER_DATA_CACHE_H__

#include <map>
#include <string>
#include <vector>
#include <time.h>

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
  		int now = time(NULL);
  		typename list<CacheKeyType>::iterator iter = father_->access_queue.begin();
			while (iter != father_->access_queue.end()){
  			if (iter->getLastAccessTime() + iter->getTimeOut() < now){
  				typename list<CacheKeyType>::iterator buf = iter;
  				iter++;
  			  father_->removeKey(buf->getKey());
				} else
          iter++;
  		}
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
    if (time_out != -1)
		  cleaner.start();
	}
/*	
	map<Key, Value> getKeys() const {
	  return cache_map;
	}
*/
  vector<Value> getValues() {
    vector<Value> value_list;
    typename map<Key, CacheEntryType>::iterator iter = cache_map.begin(); 
    while (iter != cache_map.end()) {
      value_list.push_back(iter->second.getValue());
    }
    return value_list;
  }

	Value getValue(const Key& key) {
    Value value;
		if (cache_map.count(key) != 0) {
			CacheEntryType entry = cache_map[key];
			CacheKeyType cache_key = entry.getKey();
			access_queue.remove(cache_key);
			cache_key.setLastAccessTime(atoi(getLocalTimeAsString("%Y-%m-%d %H:%M:%S").c_str()));
			access_queue.add(cache_key);
			return entry.getValue();
		} else{
			return value;
		}
	}
  bool empty() {
    return cache_map.size() == 0;
  }

	void put(const Key& key, const Value& value, int time_out){
		if (capacity_ == 0 )
		  return;
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
	}
	
	void put(const Key& key, const Value& value){
		put(key, value, time_out_);
	}
	void remove(const Key& key){
		if (cache_map.count(key) != 0) {
			access_queue.remove(cache_map[key].getKey());
			cache_map[key].erase(key);
		}
	}
	bool has(const Key& key){
		return (cache_map.count(key) != 0);
	}
	Value removeKey(const Key& key){
    Value value;
		if (cache_map.count(key) != 0) {
			value = cache_map[key].getValue();
			access_queue.remove(cache_map[key].getKey());
			cache_map.erase(key);
			return value;
		} 
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
};

#endif
