#ifndef _FLOOD_SERVER_DATA_CACHEENTRY_H__
#define _FLOOD_SERVER_DATA_CACHEENTRY_H__

template <typename Key, typename Value>
class CacheEntry{
public:
  CacheEntry() {}
  CacheEntry(Key key, Value value){
  	key_ = key;
  	value_ = value;
  }
  
  Key getKey(){
  	return key_;
  }
  
  Value getValue(){
  	return value_;
  }
private:
  Key key_;
  Value value_;
};


#endif /*CACHEENTRY_H_*/
