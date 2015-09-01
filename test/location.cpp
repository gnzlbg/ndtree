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

  // // test operator<

  // test to_int/from_int coordinate-wise and shifting

  {
    auto a = location<2>({3, 2});
    CHECK(a.to_int(0) == 2_u);
    CHECK(a.to_int(1) == 3_u);
    CHECK(a.level == 2_u);
    auto k = shift(a, 0, 1);
    CHECK(!(!k));
    a = *k;
    CHECK(a.to_int(0) == 3_u);
    CHECK(a.to_int(1) == 3_u);
    CHECK(a == location<2>({3, 3}));
  }
  {  // test to_int coordinate-wise
    auto a = location<2>({2, 1});
    CHECK(a.to_int(0) == 1_u);
    CHECK(a.to_int(1) == 2_u);

    a = *shift(a, 0, -1);
    CHECK(a.to_int(0) == 0_u);
    CHECK(a.to_int(1) == 2_u);
    CHECK(a == location<2>({2, 0}));
  }
  {  // test to_int coordinate-wise
    auto a = location<2>({3, 0, 0});
    CHECK(a.to_int(0) == 4_u);
    CHECK(a.to_int(1) == 4_u);

    a = *shift(a, 0, -1);
    a = *shift(a, 1, -1);
    CHECK(a.to_int(0) == 3_u);
    CHECK(a.to_int(1) == 3_u);

    CHECK(a == location<2>({0, 3, 3}));
  }

  {  // test to_int coordinate-wise
    auto a = location<2>({1, 1, 2});
    CHECK(a.to_int(0) == 6_u);
    CHECK(a.to_int(1) == 1_u);
  }

  {  // test to_int coordinate-wise
    auto a = location<3>({6, 5});
    CHECK(a.to_int(0) == 1_u);
    CHECK(a.to_int(1) == 2_u);
    CHECK(a.to_int(2) == 3_u);
  }

  // test float from location
  {
    {  // loc (0.25, 0.75)
      std::array<num_t, 2> x{{0.25, 0.75}};
      {
        auto fl = location<2>(x, 1);
        CHECK(fl.level == 1_u);
        CHECK(size(fl()) == 1_u);
        CHECK(fl[1] == 2_u);
        CHECK(fl()[0] == 2_u);
      }
      {
        auto fl = location<2>(x, 0);
        CHECK(fl.level == 0_u);
        CHECK(size(fl()) == 0_u);
      }
    }
    {  // loc (0.6, 0.8)
      std::array<num_t, 2> x{{0.6, 0.8}};
      {
        auto fl = location<2>(x, 1);
        CHECK(fl.level == 1_u);
        CHECK(size(fl()) == 1_u);
        CHECK(fl[1] == 3_u);
        CHECK(fl()[0] == 3_u);
      }
      {
        auto fl = location<2>(x, 2);
        CHECK(fl.level == 2_u);
        CHECK(size(fl()) == 2_u);
        CHECK(fl[1] == 3_u);
        CHECK(fl()[0] == 3_u);
        CHECK(fl[2] == 2_u);
        CHECK(fl()[1] == 2_u);
      }
      {
        auto fl = location<2>(x, 3);
        CHECK(fl.level == 3_u);
        CHECK(size(fl()) == 3_u);
        CHECK(fl[1] == 3_u);
        CHECK(fl()[0] == 3_u);
        CHECK(fl[2] == 2_u);
        CHECK(fl()[1] == 2_u);
        CHECK(fl[3] == 0_u);
        CHECK(fl()[2] == 0_u);
      }
    }
  }

  // test to_int whole

  // test shift
  /// test that shift fails

  return test::result();
}
