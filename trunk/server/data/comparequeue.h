#ifndef COMPAREQUEUE_H_
#define COMPAREQUEUE_H_

#include <list>
using namespace std;

template <typename Key>
class CompareQueue{
public:
  CompareQueue(int (*compare)(const Key&, const Key&)) {
  	compare_ = compare;
  }
  
  void add(const Key& key) {
    if (find(queue.begin(), queue.end(), key) == queue.end()) {
      typename list<Key>::iterator iter;
    	for(iter = queue.begin(); iter != queue.end(); iter++) {
    	  if (compare_(key, *iter) < 0)
    	  	break;
    	}
    	queue.insert(iter, key);
    }
  }
  
  Key removeFirst() {
    Key key;
  	if (!queue.empty()){
  	  key = *queue.begin();
  	  queue.pop_front();
  	  return key;
    }
    return key;
  }
  
  void remove(const Key& key) {
  	typename list<Key>::iterator iter = find(queue.begin(), queue.end(), key);
  	if (iter != queue.end()){
  		queue.erase(iter);
  	}
  }

  typename list<Key>::iterator erase(typename list<Key>::iterator iter) {
    return queue.erase(iter);
  }
  
  int size() {
  	return queue.size();
  }
  
  typename list<Key>::iterator begin() {
  	return queue.begin();
  }
  typename list<Key>::iterator end() {
  	return queue.end();
  }
  
  void remove(typename list<Key>::iterator iter){
  	queue.erase(iter);
  }
  
  typedef int (*Comp) (const Key&, const Key&);
private:
  list<Key> queue;
  Comp compare_;
};

#endif /*COMPAREQUEUE_H_*/
