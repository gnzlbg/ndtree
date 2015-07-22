#include "test.hpp"
#include <ndtree/location.hpp>

using namespace ndtree;

int main() {
  location<3> l;
  location<3> b;

  CHECK(l.level == 0u);
  CHECK(size(l()) == 0u);
  CHECK(l == b);
  CHECK(!(l != b));

  l = location<3>({0, 1, 0, 1});
  CHECK(l.level == 4u);
  CHECK(size(l()) == 4u);
  CHECK(l != b);
  CHECK(!(l == b));

  CHECK(l[1] == 0u);
  CHECK(l[2] == 1u);
  CHECK(l[3] == 0u);
  CHECK(l[4] == 1u);

  CHECK(l()[0] == 0u);
  CHECK(l()[1] == 1u);
  CHECK(l()[2] == 0u);
  CHECK(l()[3] == 1u);

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
