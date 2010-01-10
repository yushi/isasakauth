#ifndef __STORAGE_H__
#include <string>
using namespace std;

class Storage {
protected:
  string hostname;
  int port;
  string key_prefix;
public:
  Storage() {
    this->key_prefix = string("");
  }

  Storage(string key_prefix) {
    this->key_prefix = key_prefix;
  }

  ~Storage() {
  }

  void set_host_port(string hostname, int port) {
    this->hostname = hostname;
    this->port = port;
    return;
  }

  virtual string get(string key) {
    return string("");
  }

  virtual bool set(string key, string val) {
    return false;
  }

  virtual bool del(string key) {
    return false;
  }


};
#define __STORAGE_H__
#endif
