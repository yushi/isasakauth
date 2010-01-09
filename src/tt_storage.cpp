#include "tt_storage.h"

TTStorage::TTStorage() {
}

TTStorage::~TTStorage() {
}

string TTStorage::get(string key) {
  string ret("");
  TCRDB *db = tcrdbnew();

  if (tcrdbopen(db, (this->hostname).c_str(), this->port)) {
    int val_size = 0;
    char* val = (char*)tcrdbget(db,
                                key.c_str(),
                                key.size(),
                                &val_size);

    if (val != NULL) {
      ret = string(val, val_size);
      free(val);
    }

    if (!tcrdbclose(db)) {
    }
  } else {
  }

  tcrdbdel(db);

  return ret;
}

bool TTStorage::set(string key, string val) {
  bool ret = false;
  TCRDB *db = tcrdbnew();

  if (tcrdbopen(db, (this->hostname).c_str(), this->port)) {
    ret = (char*)tcrdbput(db,
                          key.c_str(),
                          key.size(),
                          val.c_str(),
                          val.size());

    if (!tcrdbclose(db)) {
    }
  } else {
  }

  tcrdbdel(db);
  return ret;

}

bool TTStorage::del(string key) {
  bool ret = false;
  TCRDB *db = tcrdbnew();

  if (tcrdbopen(db, (this->hostname).c_str(), this->port)) {
    ret = (char*)tcrdbout(db,
                          key.c_str(),
                          key.size());

    if (!tcrdbclose(db)) {
    }
  } else {
  }

  tcrdbdel(db);
  return ret;
}
