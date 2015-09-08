#pragma once
/// \file test.hpp Location testing functions

#include "../test.hpp"
#include <ndtree/concepts.hpp>

template <ndtree::uint_t nd, ndtree::uint_t no_levels, typename Loc>
void test_location(Loc) {
  using namespace ndtree;
  static_assert(Movable<Loc>{}, "");
  static_assert(Copyable<Loc>{}, "");
  static_assert(SemiRegular<Loc>{}, "");
  static_assert(EqualityComparable<Loc>{}, "");
  static_assert(Regular<Loc>{}, "");
  static_assert(Dimensioned<Loc>{}, "");
  static_assert(TotallyOrdered<Loc>{}, "");
  static_assert(Location<Loc>{}, "");
  Loc l;
  static_assert(l.dimension() == nd, "");
  static_assert(l.no_levels() == no_levels, "");
  static_assert(l.max_level() == no_levels - 1, "");
  test::check_equal(l.dimensions(), dimensions(nd));
  {
    auto a = l;
    uint_t count_ = 0;
    CHECK(a.level() == count_);
    // CHECK(a[0_u] == 1_u);
    for (auto&& c : view::iota(0_u, no_children(nd))) {
      a.push(c);
      count_++;
      CHECK(a.level() == count_);
      CHECK(a[count_] == c);
    }
    test::check_equal(a.levels(), view::iota(1_u, count_ + 1));
    test::check_equal(a(), view::iota(0_u, no_children(nd)));
    for_each(view::iota(0_u, no_children(nd)) | view::reverse, [&](auto&& c) {
      CHECK(a.pop() == c);
      count_--;
      CHECK(a.level() == count_);
    });
  }
  {  // test push up to max level (min pip index: 0)
    auto a = l;
    while (a.level() != a.max_level()) { a.push(0_u); }
    auto c = a;
    c.reverse();
    CHECK(c == a);
    CHECK(a.level() == a.max_level());
    CHECK(static_cast<uint_t>(a) == math::ipow(2_u, nd * a.max_level()));

    {
      auto b = a;
      std::cout << "[min start] nd: " << nd << " max_lvl: " << a.max_level()
                << std::endl;
      for (auto d : dimensions(nd)) {
        std::array<int_t, nd> offset{};
        fill(offset, int_t{0});

        auto bs = shift(b, offset);
        std::cout << "  before shift (" << d << "): " << bs << std::endl;
        CHECK(bs);

        offset[d] = -1;

        auto as = shift(b, offset);
        std::cout << "  after shift (" << d << "): " << as << std::endl;
        CHECK(!as);
      }
      std::cout << "[min end] nd: " << nd << " max_lvl: " << a.max_level()
                << std::endl;
    }
  }
  {  // test push up to max level (max pip index: no_children(nd) - 1)
    auto a = l;
    while (a.level() != a.max_level()) { a.push(no_children(nd) - 1_u); }
    CHECK(a.level() == a.max_level());
    uint_t r = 0;
    for (auto&& lvl : a.levels()) {
      for (auto&& d : dimensions(nd)) { bit::set(r, (lvl - 1) * nd + d, true); }
    }
    bit::set(r, a.level() * nd, true);

    CHECK(static_cast<uint_t>(a) == r);

    {
      auto b = a;
      std::cout << "[max start] nd: " << nd << " max_lvl: " << a.max_level()
                << std::endl;
      for (auto d : dimensions(nd)) {
        std::array<int_t, nd> offset{};
        fill(offset, int_t{0});

        auto bs = shift(b, offset);
        std::cout << "  before shift (" << d << "): " << bs << std::endl;
        CHECK(bs);

        offset[d] = 1;

        auto as = shift(b, offset);
        std::cout << "  after shift (" << d << "): " << as << std::endl;
        CHECK(!as);
      }
      std::cout << "[max end] nd: " << nd << " max_lvl: " << a.max_level()
                << std::endl;
    }
  }
}

template <template <ndtree::uint_t, class...> class Loc>
void test_location_2() {
  using namespace ndtree;
  {
    Loc<3> l;
    Loc<3> b;

    CHECK(l.level() == 0_u);
    CHECK(size(l()) == 0_u);
    CHECK(l == b);
    CHECK(!(l != b));

    l = Loc<3>({0, 1, 0, 1});
    CHECK(l.level() == 4_u);
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
    CHECK(b == Loc<3>({1, 0, 1, 0}));
    b.reverse();
    CHECK(b == l);
  }

  {
    auto a = Loc<2>({3, 2});
    CHECK(static_cast<uint_t>(a) == 30_u);
    std::array<uint_t, 2> ar(a);
    CHECK(ar[0] == 2_u);
    CHECK(ar[1] == 3_u);
    CHECK(a.level() == 2_u);
    auto k = shift(a, std::array<int_t, 2>{{1, 0}});
    CHECK(!(!k));
    a = *k;
    ar = static_cast<std::array<uint_t, 2>>(a);
    CHECK(ar[0] == 3_u);
    CHECK(ar[1] == 3_u);
    CHECK(a == Loc<2>({3, 3}));
  }
  {  // test to_int coordinate-wise
    auto a = Loc<2>({2, 1});
    std::array<uint_t, 2> ar(a);
    CHECK(ar[0] == 1_u);
    CHECK(ar[1] == 2_u);

    a = *shift(a, std::array<int_t, 2>{{-1, 0}});
    ar = static_cast<std::array<uint_t, 2>>(a);
    CHECK(ar[0] == 0_u);
    CHECK(ar[1] == 2_u);
    CHECK(a == Loc<2>({2, 0}));
  }
  {  // test to_int coordinate-wise
    auto a = Loc<2>({3, 0, 0});
    std::array<uint_t, 2> ar(a);
    CHECK(ar[0] == 4_u);
    CHECK(ar[1] == 4_u);

    a = *shift(a, std::array<int_t, 2>{{-1, -1}});
    ar = static_cast<std::array<uint_t, 2>>(a);
    CHECK(ar[0] == 3_u);
    CHECK(ar[1] == 3_u);

    CHECK(a == Loc<2>({0, 3, 3}));
  }
  {  // test push up to max level
    Loc<2> l;
    for (uint_t i = 0; i < l.max_level(); ++i) { l.push(0_u); }
    CHECK(l.level() == l.max_level());
  }

  {  // test to_int coordinate-wise
    auto a = Loc<2>({1, 1, 2});
    std::array<uint_t, 2> ar(a);
    CHECK(ar[0] == 6_u);
    CHECK(ar[1] == 1_u);
  }

  {  // test to_int coordinate-wise
    auto a = Loc<3>({6, 5});
    std::array<uint_t, 3> ar(a);
    CHECK(a.level() == 2_u);
    CHECK(ar[0] == 1_u);
    CHECK(ar[1] == 2_u);
    CHECK(ar[2] == 3_u);
  }

  // test float from Loc
  {
    {  // loc (0.25, 0.75)
      std::array<num_t, 2> x{{0.25, 0.75}};
      {
        auto fl = Loc<2>(x, 1);
        CHECK(fl.level() == 1_u);
        CHECK(size(fl()) == 1_u);
        CHECK(fl[1] == 2_u);
        CHECK(fl()[0] == 2_u);
      }
      {
        auto fl = Loc<2>(x, 0);
        CHECK(fl.level() == 0_u);
        CHECK(size(fl()) == 0_u);
      }
    }
    {  // loc (0.6, 0.8)
      std::array<num_t, 2> x{{0.6, 0.8}};
      {
        auto fl = Loc<2>(x, 1);
        CHECK(fl.level() == 1_u);
        CHECK(size(fl()) == 1_u);
        CHECK(fl[1] == 3_u);
        CHECK(fl()[0] == 3_u);
      }
      {
        auto fl = Loc<2>(x, 2);
        CHECK(fl.level() == 2_u);
        CHECK(size(fl()) == 2_u);
        CHECK(fl[1] == 3_u);
        CHECK(fl()[0] == 3_u);
        CHECK(fl[2] == 2_u);
        CHECK(fl()[1] == 2_u);
      }
      {
        auto fl = Loc<2>(x, 3);
        CHECK(fl.level() == 3_u);
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
}
