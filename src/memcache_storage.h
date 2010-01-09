#ifndef __MEMCACHE_STORAGE_H__

#include "storage.h"
#include <string>
#include <libmemcached/memcached.h>

using namespace std;

class MemcacheStorage : public Storage {
 public:
  MemcacheStorage();
  ~MemcacheStorage();
  string get(string key);
  bool set(string key, string val);
  bool del(string key);
 private:
  memcached_st* get_memc();
};

#define __MEMCACHE_STORAGE_H__
#endif
