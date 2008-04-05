#include "server/util/calulate.h"

#include "base/util.h"
#include "base/flags.h"
#include "base/logging.h"

string getIp(unsigned int ip_) {
  unsigned int a, b, c, d;
  a = ip_ % 256;
  ip_ /= 256;
  b = ip_ % 256;
  ip_ /= 256;
  c = ip_ % 256;
  ip_ /= 256;
  d = ip_ % 256;
  return stringPrintf("%u.%u.%u.%u", d, c, b, a);
}

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

