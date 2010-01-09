extern "C" {
#include <ngx_config.h>
#include <ngx_core.h>
#include <ngx_event.h>
#include <ngx_event_connect.h>
#include <ngx_event_pipe.h>
#include <ngx_http.h>
#include "ddebug.h"
}

#include <string>
#include <iostream>
#include "session.h"
#include "tt_storage.h"
using namespace std;

string* get_param(string postdata, string key);

class Authenticator {
private:
  string hostname;
  int port;
  TTStorage storage;

public:
  Authenticator(string hostname, int port) {
    this->storage.set_host_port(hostname, port);
  }

  ~Authenticator() {
  }

  bool is_valid_account(string username, string password) {
    bool ret = false;

    if (username == string("")) {
      return ret;
    }

    if (password == string("")) {
      return ret;
    }

    string real_pass = this->storage.get(username);

    if (real_pass == string("")) {
      return ret;

    }

    if (password == real_pass) {
      ret = true;
    }

    return ret;
  }

};


