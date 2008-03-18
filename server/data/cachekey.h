#ifndef _FLOOD_SERVER_DATA_CACHEKEY_H__
#define _FLOOD_SERVER_DATA_CACHEKEY_H__

template <typename Key>
class CacheKey{
public:
  CacheKey(Key key, int last_asscess_time, int time_out);
  
  Key getKey() const;
  int getLastAsscessTime() const;
  int getTimeOut() const;
  
  void setKey(Key key);
  void setLastAsscessTime(int last_asscess_time);
  void setTimeOut(int time_out);
  
private:
  Key key_;
  int last_asscess_time_;
  int time_out_;
};

template <typename Key>
CacheKey<Key>::CacheKey(Key key, int last_asscess_time, int time_out)
  : key_(key), last_asscess_time_(last_asscess_time), time_out_(time_out) {
}

template <typename Key>
Key CacheKey<Key>::getKey() const {
  return key_;
}
template <typename Key>
int  CacheKey<Key>::getLastAsscessTime() const {
	return last_asscess_time_;
}

template <typename Key>
int  CacheKey<Key>::getTimeOut() const {
	return time_out_;
}

template <typename Key>
void  CacheKey<Key>::setKey(Key key) {
	key_ = key;
}

template <typename Key>
void  CacheKey<Key>::setLastAsscessTime(int last_asscess_time) {
	last_asscess_time_ = last_asscess_time;
}

template <typename Key>
void  CacheKey<Key>::setTimeOut(int time_out) {
	time_out_ = time_out;
}


#endif /*CACHEKEY_H_*/
