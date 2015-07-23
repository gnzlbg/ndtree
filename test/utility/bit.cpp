#include "../test.hpp"
#include <ndtree/types.hpp>
#include <ndtree/utility/bit.hpp>

using namespace ndtree;

int main() {
  uint_t a = 0;
  CHECK(bit::to_int(a) == a);
  CHECK(bit::to_int(a, 0_u, 3_u) == a);
  for (auto&& i : bit::bits<uint_t>()) { CHECK(bit::get(a, i) == false); }

  uint_t b = std::numeric_limits<uint_t>::max();
  CHECK(bit::to_int(b) == b);
  CHECK(bit::to_int(b, 0_u, 3_u) == 7_u);
  for (auto&& i : bit::bits<uint_t>()) { CHECK(bit::get(b, i) == true); }

  for (auto&& i : bit::bits<uint_t>()) { bit::set(a, i, true); }
  CHECK(a == b);

  for (auto&& i : bit::bits<uint_t>()) { bit::set(b, i, false); }
  CHECK(b == 0_u);

  return test::result();
}
