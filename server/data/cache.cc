/*
#ifndef _FLOOD_SERVER_DATA_CACHE_CC__
#define _FLOOD_SERVER_DATA_CACHE_CC__

#include "inc.h"
#include "cache.h"

template <typename Value, typename Key>
int Cache <Value, Key>:: put(const Value& value, 
                             const Key& key){
  cache_map.insert(make_pair(key,value));
  return 1;
}

template <typename Value, typename Key>
int Cache <Value, Key>:: deleteKey(const Key& key){
  if (cache_map.erase(key))
    return 1;
  else
    return 0;
};

template <typename Value, typename Key>
Value Cache <Value, Key>:: get(const Key& key){
  if (cache_map.count(key))
    return cache_map[key];
  return Value();
}

template <typename Value, typename Key>
int Cache <Value, Key>:: update(const Value& value,
                                const Key& key){
  if (!cache_map.count(key))
    return 0;
  cache_map[key]=value;
  return 1;
}

#endif

*/