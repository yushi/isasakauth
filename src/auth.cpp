#include "auth.h"

string* get_param(string postdata, string key) {
  key += string("=");
  size_t start, end;
  start = postdata.find(key);
  end = postdata.find("&", start);

  if (start  == string::npos) {
    return NULL;
  }

  start +=  key.size();

  if (end == string::npos) {
    return new string(postdata.substr(start));
  }

  return new string(postdata.substr(start, end - start));
}
