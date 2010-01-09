#include <uuid/uuid.h>
#include <string>
#include "exceptions.h"
using namespace std;

class SessionID {
public:
  uuid_t uuid;
  SessionID() {
    uuid_generate(this->uuid);
  }

  SessionID(uuid_t id) {
    uuid_copy(this->uuid, id);
  }

  SessionID(string session_id) {
    int result = uuid_parse(session_id.c_str(), this->uuid);

    if (result == -1) {
      throw invalid_session();
    }
  }

  ~SessionID() {
  }
  
  string to_s() {
    char uuid_str[37];
    uuid_unparse(this->uuid, uuid_str);
    return string(uuid_str);

  }

};
