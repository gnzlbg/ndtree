#include "../test.hpp"
#include <ndtree/types.hpp>
#include <ndtree/utility/bit.hpp>

using namespace ndtree;

template <
 typename ValueType, typename OffsetType,
 CONCEPT_REQUIRES_(UnsignedIntegral<ValueType>{}
                   and bit::width<ValueType> == bit::width<OffsetType>)>
void check_overflows_on_add() {
  ValueType v0 = 0;
  ValueType v1 = 1;
  ValueType v2 = 2;
  ValueType vm = std::numeric_limits<ValueType>::max();
  OffsetType o0 = 0;
  OffsetType o1 = 1;
  OffsetType o2 = 2;
  OffsetType om = std::numeric_limits<OffsetType>::max();

  // zero offset:
  CHECK(!bit::overflows_on_add(v0, v0));
  CHECK(!bit::overflows_on_add(v0, o0));
  CHECK(!bit::overflows_on_add(v1, v0));
  CHECK(!bit::overflows_on_add(v1, o0));
  CHECK(!bit::overflows_on_add(v2, v0));
  CHECK(!bit::overflows_on_add(v2, o0));
  CHECK(!bit::overflows_on_add(vm, v0));
  CHECK(!bit::overflows_on_add(vm, o0));

  // +1 offset:
  CHECK(!bit::overflows_on_add(v0, v1));
  CHECK(!bit::overflows_on_add(v0, o1));
  CHECK(!bit::overflows_on_add(v1, v1));
  CHECK(!bit::overflows_on_add(v1, o1));
  CHECK(!bit::overflows_on_add(v2, v1));
  CHECK(!bit::overflows_on_add(v2, o1));
  CHECK(bit::overflows_on_add(vm, v1));
  CHECK(bit::overflows_on_add(vm, o1));

  // +2 offset
  CHECK(!bit::overflows_on_add(v0, v2));
  CHECK(!bit::overflows_on_add(v0, o2));
  CHECK(!bit::overflows_on_add(v1, v2));
  CHECK(!bit::overflows_on_add(v1, o2));
  CHECK(!bit::overflows_on_add(v2, v2));
  CHECK(!bit::overflows_on_add(v2, o2));
  CHECK(bit::overflows_on_add(vm, v2));
  CHECK(bit::overflows_on_add(vm, o2));

  // +m offset
  CHECK(!bit::overflows_on_add(v0, vm));
  CHECK(!bit::overflows_on_add(v0, om));
  CHECK(bit::overflows_on_add(v1, vm));
  CHECK(bit::overflows_on_add(v2, vm));
  CHECK(bit::overflows_on_add(vm, vm));
  CHECK(bit::overflows_on_add(vm, om));
  if (UnsignedIntegral<OffsetType>{}) {
    CHECK(bit::overflows_on_add(v1, om));
    CHECK(bit::overflows_on_add(v2, om));
  } else {
    CHECK(!bit::overflows_on_add(v1, om));
    CHECK(!bit::overflows_on_add(v2, om));
  }

  // Overflow on substract checks
  if (SignedIntegral<OffsetType>{}) {
    // zero offset:
    CHECK(!bit::overflows_on_add(v0, -o0));
    CHECK(!bit::overflows_on_add(v1, -o0));
    CHECK(!bit::overflows_on_add(v2, -o0));
    CHECK(!bit::overflows_on_add(vm, -o0));

    // -1 offset:
    CHECK(bit::overflows_on_add(v0, -o1));
    CHECK(!bit::overflows_on_add(v1, -o1));
    CHECK(!bit::overflows_on_add(v2, -o1));
    CHECK(!bit::overflows_on_add(vm, -o1));

    // -2 offset
    CHECK(bit::overflows_on_add(v0, -o2));
    CHECK(bit::overflows_on_add(v1, -o2));
    CHECK(!bit::overflows_on_add(v2, -o2));
    CHECK(!bit::overflows_on_add(vm, -o2));

    // -m offset
    CHECK(bit::overflows_on_add(v0, -om));
    CHECK(bit::overflows_on_add(v1, -om));
    CHECK(bit::overflows_on_add(v2, -om));
    CHECK(!bit::overflows_on_add(vm, -om));
  }
}

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
    check_overflows_on_add<uint32_t, uint32_t>();
    check_overflows_on_add<uint32_t, int32_t>();
    check_overflows_on_add<uint64_t, uint64_t>();
    check_overflows_on_add<uint64_t, int64_t>();
    check_overflows_on_add<unsigned int, unsigned int>();
    check_overflows_on_add<unsigned int, int>();
  }
  return test::result();
}
