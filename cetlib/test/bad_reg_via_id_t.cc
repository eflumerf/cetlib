#include "cetlib/registry_via_id.h"

using cet::registry_via_id;

/*
* This test is meant to fail due to the registry
* being used without a proper id() function in 
* the value type
*/

struct thing {
  int i;
  thing(int i = 0) : i(i) {}
  // float
  // id() const
  // {
  //   return i;
  // }
  bool
  operator==(int other) const
  {
    return i == other;
  }
};


int main(){
  using reg = registry_via_id<float, thing>;
}
