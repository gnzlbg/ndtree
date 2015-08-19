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

  {  // check full number
    uint_t c = 2;
    bool bits[bit::width<uint_t>];
    for (auto&& i : bit::bits<uint_t>()) { bits[i] = false; }
    bits[1] = true;

    for (auto&& i : bit::bits<uint_t>()) { CHECK(bit::get(c, i) == bits[i]); }
    CHECK(bit::to_int(c) == 2_u);
  }
  {  // check full number
    uint_t c = 3;
    bool bits[bit::width<uint_t>];
    for (auto&& i : bit::bits<uint_t>()) { bits[i] = false; }
    bits[0] = true;
    bits[1] = true;

    for (auto&& i : bit::bits<uint_t>()) { CHECK(bit::get(c, i) == bits[i]); }
    CHECK(bit::to_int(c) == 3_u);
  }

  {  // check partial number
    uint_t c;
    for (auto&& i : bit::bits<uint_t>()) { bit::set(c, i, false); }
    bit::set(c, 2_u, true);

    CHECK(bit::to_int(c, 1_u, 3_u) == 2_u);
  }

  {  // check partial number
    uint_t c;
    for (auto&& i : bit::bits<uint_t>()) { bit::set(c, i, false); }
    bit::set(c, 1_u, true);
    bit::set(c, 2_u, true);

    CHECK(bit::to_int(c, 1_u, 3_u) == 3_u);
  }

  {  // check overflows_on_add
    uint_t v0 = 0;
    uint_t v1 = 1;
    uint_t v2 = 2;
    uint_t vm1 = -1;
    uint_t vm2 = -2;

    CHECK(!bit::overflows_on_add(v0, 0));
    CHECK(!bit::overflows_on_add(v1, 0));
    CHECK(!bit::overflows_on_add(v2, 0));
    CHECK(!bit::overflows_on_add(vm1, 0));
    CHECK(!bit::overflows_on_add(vm2, 0));

    CHECK(!bit::overflows_on_add(v0, 1));
    CHECK(!bit::overflows_on_add(v0, 10));
    CHECK(bit::overflows_on_add(v0, -1));
    CHECK(bit::overflows_on_add(v1, -2));
    CHECK(!bit::overflows_on_add(v0, std::numeric_limits<uint_t>::max() - 1_u));
    CHECK(!bit::overflows_on_add(v0, std::numeric_limits<uint_t>::max()));

    CHECK(!bit::overflows_on_add(v1, 1));
    CHECK(!bit::overflows_on_add(v1, 10));
    CHECK(!bit::overflows_on_add(v1, -1));
    CHECK(bit::overflows_on_add(v1, -2));
    CHECK(!bit::overflows_on_add(v1, std::numeric_limits<uint_t>::max() - 1_u));
    CHECK(bit::overflows_on_add(v1, std::numeric_limits<uint_t>::max()));

    CHECK(!bit::overflows_on_add(v2, 1));
    CHECK(!bit::overflows_on_add(v2, 10));
    CHECK(!bit::overflows_on_add(v2, -1));
    CHECK(!bit::overflows_on_add(v2, -2));
    CHECK(bit::overflows_on_add(v2, -3));
    CHECK(bit::overflows_on_add(v2, std::numeric_limits<uint_t>::max() - 1_u));
    CHECK(bit::overflows_on_add(v2, std::numeric_limits<uint_t>::max()));

    CHECK(bit::overflows_on_add(vm1, 1));
    CHECK(bit::overflows_on_add(vm1, 10));
    CHECK(bit::overflows_on_add(vm1, std::numeric_limits<uint_t>::max() - 1_u));
    CHECK(bit::overflows_on_add(vm1, std::numeric_limits<uint_t>::max()));

    CHECK(!bit::overflows_on_add(vm2, 1));
    CHECK(bit::overflows_on_add(vm2, 10));
    CHECK(bit::overflows_on_add(vm2, std::numeric_limits<uint_t>::max() - 1_u));
    CHECK(bit::overflows_on_add(vm2, std::numeric_limits<uint_t>::max()));
  }

  return test::result();
}
