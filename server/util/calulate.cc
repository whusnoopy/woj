#include "server/util/calulate.h"

string calIndentifyCode(const string& key){
  return key;
}

void spriteString(const string& buf, char separator, vector<string>& output) {
  int k = 0;
  for (int i = 0; i < buf.size(); i++) {
    if (buf[i] == separator) {
      if (i > k) {
        output.push_back(buf.substr(k,i-k));
      }
      k = i+1;
    }
  }
  if (k < buf.size()) {
    output.push_back(buf.substr(k, buf.size() - k));
  }
}

