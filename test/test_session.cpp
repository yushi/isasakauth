#include "../src/session.h"
#include <iostream>
using namespace std;

int main() {
  Session first = Session(string("localhost"), 1978, string("yushi"));
  cout << "create new session:" << first.username << "->" << first.session_id.to_s() << endl;

  Session second = Session(string("localhost"), 1978, first.session_id);
  cout << "get username from session:" << second.username << endl;
  bool result = second.expire();
  cout << "expire:" << result << endl;

  try {
    Session third = Session(string("localhost"), 1978, first.session_id);
  } catch (invalid_session e) {
    cout << "expired session:" << first.session_id.to_s() << endl;
  }


}

