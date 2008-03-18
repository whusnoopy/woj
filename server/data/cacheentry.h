#ifndef _FLOOD_SERVER_DATA_CACHEENTRY_H__
#define _FLOOD_SERVER_DATA_CACHEENTRY_H__

#include "cachekey.h"

template <typename Key, typename Value>
class CacheEntry{
public:
  CacheEntry(CacheKey<Key> key, Value value){
  	key_ = key;
  	value_ = value;
  }
  
  Cache<Key> getKey(){
  	return key_;
  }
  
  Value getValue(){
  	return value_;
  }
private:
  const CacheKey<Key> key_;
  const Value value_;
};


#endif /*CACHEENTRY_H_*/
