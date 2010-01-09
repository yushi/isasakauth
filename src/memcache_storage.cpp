#include "memcache_storage.h"
#include <iostream>
MemcacheStorage::MemcacheStorage() {
}

MemcacheStorage::~MemcacheStorage() {
}


string MemcacheStorage::get(string key) {
  string ret("");
  memcached_st *memc;
  memcached_return rc;
  uint32_t flag;
  
  memc = this->get_memc();
  
  if(memc == NULL){
    return ret;
  }
  size_t val_size = 0;
  char *val = (char*)memcached_get(memc,
                                   key.c_str(),
                                   key.size(),
                                   &val_size,
                                   &flag,
                                   &rc);
  if (val != NULL) {
    ret = string(val, val_size);
    free(val);
  }


  memcached_free(memc);

  return ret;
}

bool MemcacheStorage::set(string key, string val) {
  bool ret = false;
  memcached_st *memc;
  memcached_return rc;
  uint32_t flag = 0;
  memc = this->get_memc();
  
  if(memc == NULL){
    return ret;
  }

  rc = memcached_set (memc,
                      key.c_str(),
                      key.size(),
                      val.c_str(),
                      val.size(),
                      (time_t)0,
                      flag);

  if(rc == MEMCACHED_SUCCESS){
    ret = true;
  }
  
  memcached_free(memc);
  return ret;

}

bool MemcacheStorage::del(string key) {
  bool ret = false;
  memcached_st *memc;
  memcached_return rc;
  memc = this->get_memc();

  if(memc == NULL){
    return ret;
  }

  rc = memcached_delete (memc,
                         key.c_str(),
                         key.size(),
                         (time_t)0);  

  if(rc == MEMCACHED_SUCCESS){
    ret = true;
  }

  memcached_free(memc);
  return ret;
}

memcached_st* MemcacheStorage::get_memc(){
  memcached_st *ret = NULL;
  memcached_server_st *server = NULL;
  memcached_return rc;
  ret = memcached_create(NULL);

  if(ret == NULL){
    return ret;
  }
  
  server = memcached_server_list_append(NULL,
                                     this->hostname.c_str(),
                                     this->port,
                                     &rc);
  rc = memcached_server_push(ret, server);
  memcached_server_list_free(server);
  return ret;
}
