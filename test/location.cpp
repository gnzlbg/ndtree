#include "test.hpp"
#include <ndtree/location.hpp>

using namespace ndtree;

int main() {
  location<3> l;
  location<3> b;

  CHECK(l.level == 0_u);
  CHECK(size(l()) == 0_u);
  CHECK(l == b);
  CHECK(!(l != b));

  l = location<3>({0, 1, 0, 1});
  CHECK(l.level == 4_u);
  CHECK(size(l()) == 4_u);
  CHECK(l != b);
  CHECK(!(l == b));

  CHECK(l[1] == 0_u);
  CHECK(l[2] == 1_u);
  CHECK(l[3] == 0_u);
  CHECK(l[4] == 1_u);

  CHECK(l()[0] == 0_u);
  CHECK(l()[1] == 1_u);
  CHECK(l()[2] == 0_u);
  CHECK(l()[3] == 1_u);

  b = l;
  CHECK(b == l);
  b.reverse();
  CHECK(b != l);
  l = location<3>({1, 0, 1, 0});
  CHECK(b == l);

  // test operator<

  // test to_int coordinate

  // test to_int whole

  // test shift


  return test::result();
}
