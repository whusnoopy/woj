#ifndef _FLOOD_SERVER_DATA_CACHE_H__
#define _FLOOD_SERVER_DATA_CACHE_H__

#include <map>
#include <string>

#include <time.h>

#include "cacheentry.h"
#include "cachekey.h"
#include "comparequeue.h"
#include "util/thread.h"
using namespace std;

template <typename Key, typename Value>
class Cache{
public:
	typedef CacheKey<Key> CacheKeyType;
	typedef CacheEntry<Key, Value> CacheEntryType;
	Cache(int time_out, int capacity) {
		time_out_ = time_out;
		capacity_ = capacity;
		queue = Queue();
		clear.start();
	}
	
	map<Key, Value> getKeys() const {
	  return cache_map;
	}
	
	Value getValue(const Key& key) {
		if (cache_map.count(key) != 0) {
			CacheEntryType entry = cache_map[key];
			CacheKeyType cache_key = entry.getKey();
			access_queue.remove(cache_key);
			cache_key.setLastAccessTime();
			access_queue.add(cache_key);
			return entry.getValue();
		} else{
			return Value;
		}
	}
	
	void put(const Key& key, const Value& value, int time_out){
		if (capacity == 0 )
		  return;
		CacheKeyType cache_key(key, time(), time_out);
		CacheEntryType entry(cache_key, value);
		if (cache_map.count(key) == 0) {
			if (cache_map.size() >= capacity){
				CacheKeyType remove_key = access_queue.removeFirst();
				cache_map.erase(remove_key.getKey());
			}
		}else {
		  access_queue.remove(cache_map[key].getKey());
		}
		cache_map[key] = entry;
		accesstime.add(cache_key);
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
		if (cache_map.count(key) != 0) {
			Value value = cache_map[key].getValue();
			access_queue.remove(cache_map[key].getKey());
			cache_map[key].erase(key);
			return value;
		} 
		return Value;
	}
  
  static int compare(const CacheKeyType& a, const CacheKeyType& b) {
  	return (a.getLastAccessTime() + a.getTimeOut()) 
  	       - (b.getLastAccessTime() + b.getTimeOut());
  }
  
  void join() {
  	cleaner.join();
  }

private:
  map<Key, CacheEntry> cache_map;
  CompareQueue<CacheKeyType> access_queue;
  
  class CacheCleaner : public Thread{
  public:
  	CacheCleaner() { quitflag = false; }
  	void running(){
  		while (!quitflag){
  			int now = time();
  			vector<CacheKeyType>::iterator iter = access_queue.begin();
  			while (iter != access_queue.end()){
  				if (iter->getLastAccessTime() + iter->getTimeOut() < now){
  					vector<CacheKeyType>::iterator buf = iter;
  					iter++;
  				  access_queue.remove(buf);
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
  };
  CacheCleaner cleaner;
};

#endif
