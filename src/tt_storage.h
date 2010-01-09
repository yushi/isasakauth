#ifndef __TT_STORAGE_H__

#include "storage.h"
#include <tcrdb.h>
#include <string>

using namespace std;

class TTStorage : public Storage {
 public:
  TTStorage();
  ~TTStorage();
  string get(string key);
  bool set(string key, string val);
  bool del(string key);
};

#define __TTSTORAGE_H__
#endif
