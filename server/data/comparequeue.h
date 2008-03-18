#ifndef COMPAREQUEUE_H_
#define COMPAREQUEUE_H_

template <typename Key>
class CompareQueue{
public:
  CompareQueue(Comp compare) {
  	compare_ = compare;
  }
  
  void add(const Key& key) {
    if (find(queue.begin(), queue.end(), key) == queue.end()) {
    	for(vector<Key>::iterator iter = queue.begin(); iter != queue.end(); iter++) {
    	  if (compare_(key, *iter) < 0)
    	  	break;
    	}
    	queue.insert(iter, key);
    }
  }
  
  Key removeFirst() {
  	if (!queue.empty()){
  	  Key key = *queue.begin();
  	  queue.pop_front();
  	  return key;
    }
    return Key;
  }
  
  void remove(const Key& key) {
  	vector<Key>::iterator iter = find(queue.begin(), queue.end(), key);
  	if (iter != queue.end()){
  		queue.erase(iter);
  	}
  }
  
  int size() {
  	return queue.size();
  }
  
  vector<Key>::iterator begin() {
  	return queue.begin();
  }
  vector<Key>::iterator end() {
  	return queue.end();
  }
  
  void remove(vector<Key>::iterator iter){
  	queue.erase(iter);
  }
  
  typedef int (*Comp) (const Key&, const Key&);
private:
  vector<Key> queue;
  Comp compare_;
};

#endif /*COMPAREQUEUE_H_*/
