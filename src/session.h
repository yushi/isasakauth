
extern "C" {
#include "ddebug.h"
}

#include <string>
#include <iostream>
#include <tcrdb.h>
#include "session_id.h"
#include "memcache_storage.h"

using namespace std;

class Session {
public:
  SessionID session_id;
  string username;
private:
  MemcacheStorage storage;

public:
  Session(string host, int port, SessionID session_id) {
    this->storage.set_host_port(host, port);
    this->session_id = session_id;
    this->username = this->storage.get((this->session_id).to_s());

    if (this->username == string("")) {
      throw invalid_session();
    }
  }

  Session(string host, int port, string username) {
    this->storage.set_host_port(host, port);
    this->username = username;
    this->session_id = SessionID();

    this->storage.set((this->session_id).to_s(), this->username);

  }

  ~Session() {
  }

  bool expire() {
    return this->storage.del(this->session_id.to_s());
  }


private:
};


/*
  kvs info
  session_id -> user
 */
