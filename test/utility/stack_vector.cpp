/// \file stack_vector.cpp Test for stack vector
///
/// Adapted from libc++: https://libcxx.llvm.org
/// under the following license:
//===----------------------------------------------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
#include "../test.hpp"
#include <ndtree/types.hpp>
#include <ndtree/utility/stack_vector.hpp>

using namespace ndtree;

int main() {
  {  // const
    stack_vector<const int, 3> v = {1, 2, 3};
    CHECK(v[0] == 1);
    CHECK(v[1] == 2);
    CHECK(v[2] == 3);
  }

  auto test_contiguous = [](auto&& c) {
    for (size_t i = 0; i < c.size(); ++i) {
      CHECK(*(c.begin() + i) == *(std::addressof(*c.begin()) + i));
    }
  };

  {  // contiguous
    typedef int T;
    typedef stack_vector<T, 3> C;
    test_contiguous(C());
    test_contiguous(C(3, 5));
  }

  {  // default construct element
    typedef int T;
    typedef stack_vector<T, 3> C;
    C c(1);
    CHECK(back(c) == 0);
    CHECK(front(c) == 0);
    CHECK(c[0] == 0);
  }

  {  // iterator
    typedef int T;
    typedef stack_vector<T, 3> C;
    C c;
    C::iterator i = begin(c);
    C::iterator j = end(c);
    CHECK(distance(i, j) == 0);
    CHECK(i == j);
  }
  {  // const iterator
    typedef int T;
    typedef stack_vector<T, 3> C;
    const C c{};
    C::const_iterator i = begin(c);
    C::const_iterator j = end(c);
    CHECK(distance(i, j) == 0);
    CHECK(i == j);
  }
  {  // cbegin/cend
    typedef int T;
    typedef stack_vector<T, 3> C;
    C c;
    C::const_iterator i = cbegin(c);
    C::const_iterator j = cend(c);
    CHECK(distance(i, j) == 0);
    CHECK(i == j);
    CHECK(i == end(c));
  }
  {  // range constructor
    typedef int T;
    typedef stack_vector<T, 10> C;
    const T t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    C c(t);
    test::check_equal(t, c);
    C::iterator i = begin(c);
    CHECK(*i == 0);
    ++i;
    CHECK(*i == 1);
    *i = 10;
    CHECK(*i == 10);
    CHECK(distance(c) == 10);
  }
  {  // iterator constructor
    typedef int T;
    typedef stack_vector<T, 10> C;
    const T t[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    C c(begin(t), end(t));
    test::check_equal(t, c);
    C::iterator i = begin(c);
    CHECK(*i == 0);
    ++i;
    CHECK(*i == 1);
    *i = 10;
    CHECK(*i == 10);
    CHECK(distance(c) == 10);
  }
  {  // N3644 testing
    typedef stack_vector<int, 10> C;
    C::iterator ii1{}, ii2{};
    C::iterator ii4 = ii1;
    C::const_iterator cii{};
    CHECK(ii1 == ii2);
    CHECK(ii1 == ii4);

    CHECK(!(ii1 != ii2));

    CHECK((ii1 == cii));
    CHECK((cii == ii1));
    CHECK(!(ii1 != cii));
    CHECK(!(cii != ii1));
    CHECK(!(ii1 < cii));
    CHECK(!(cii < ii1));
    CHECK((ii1 <= cii));
    CHECK((cii <= ii1));
    CHECK(!(ii1 > cii));
    CHECK(!(cii > ii1));
    CHECK((ii1 >= cii));
    CHECK((cii >= ii1));
    CHECK((cii - ii1) == 0);
    CHECK((ii1 - cii) == 0);
  }
  {  // types
    auto check_types = [](auto&& c, auto&& t) {
      using C = uncvref_t<decltype(c)>;
      using T = uncvref_t<decltype(t)>;
      static_assert(Container<C>{}, "");
      static_assert(Same<meta::_t<value_type<C>>, T>{}, "");
      // static_assert(Same<meta::_t<size_type<C>>, std::size_t>{}, "");
      static_assert(Same<meta::_t<difference_type<C>>, std::ptrdiff_t>{}, "");
      static_assert(Same<typename C::reference, T&>{}, "");
      static_assert(Same<typename C::const_reference, T const&>{}, "");
      static_assert(Same<typename C::pointer, T*>{}, "");
      static_assert(Same<typename C::const_pointer, T const*>{}, "");
      static_assert(
       Same<
        typename std::iterator_traits<typename C::iterator>::iterator_category,
        std::random_access_iterator_tag>{},
       "");
      static_assert(Same<typename std::iterator_traits<
                          typename C::const_iterator>::iterator_category,
                         std::random_access_iterator_tag>{},
                    "");
      static_assert(std::is_nothrow_default_constructible<C>::value, "");

      static_assert(std::is_nothrow_move_constructible<C>::value, "");

    };

    using m = stack_vector<int, 10>;
    using c = const m;
    check_types(m{}, int{});
    check_types(c{}, int{});

    using mm = stack_vector<std::unique_ptr<int>, 10>;
    using mc = const mm;
    check_types(mm{}, std::unique_ptr<int>{});
    check_types(mc{}, std::unique_ptr<int>{});
  }

  {  // capacity
    stack_vector<int, 10> a;
    CHECK(a.capacity() == 10_u);
    a.push_back(0);
    CHECK(a.capacity() == 10_u);
  }

  {
   // erase
  }

  {  // resize copyable
    using Copyable = int;
    stack_vector<Copyable, 10> a(10_u, 5);
    CHECK(a.size() == 10_u);
    CHECK(a.capacity() == 10_u);
    test_contiguous(a);
    CHECK(a[0] == 5);
    CHECK(a[9] == 5);
    a.resize(5);
    CHECK(a.size() == 5_u);
    CHECK(a.capacity() == 10_u);
    test_contiguous(a);
    a.resize(9);
    CHECK(a[4] == 5);
    CHECK(a[5] == 0);
    CHECK(a[8] == 0);
    CHECK(a.size() == 9_u);
    CHECK(a.capacity() == 10_u);
    test_contiguous(a);
    a.resize(10, 3);
    CHECK(a[4] == 5);
    CHECK(a[8] == 0);
    CHECK(a[9] == 3);
    CHECK(a.size() == 10_u);
    CHECK(a.capacity() == 10_u);
    test_contiguous(a);
  }

  {  // resize move-only
    using MoveOnly = std::unique_ptr<int>;
    stack_vector<MoveOnly, 10> a(10);
    CHECK(a.size() == 10_u);
    CHECK(a.capacity() == 10_u);
    a.resize(5);
    CHECK(a.size() == 5_u);
    CHECK(a.capacity() == 10_u);
    a.resize(9);
    CHECK(a.size() == 9_u);
    CHECK(a.capacity() == 10_u);
  }

  {  // assign copy
    stack_vector<int, 3> a = {0, 1, 2};
    CHECK(a.size() == 3_u);
    stack_vector<int, 3> b;
    CHECK(b.size() == 0_u);
    b = a;
    CHECK(b.size() == 3_u);
    test::check_equal(a, b);
  }

  {  // copy construct
    stack_vector<int, 3> a = {0, 1, 2};
    CHECK(a.size() == 3_u);
    stack_vector<int, 3> b(a);
    CHECK(b.size() == 3_u);

    test::check_equal(a, b);
  }

  {  // assign move
    using MoveOnly = std::unique_ptr<int>;
    stack_vector<MoveOnly, 3> a(3);
    CHECK(a.size() == 3_u);
    stack_vector<MoveOnly, 3> b;
    CHECK(b.size() == 0_u);
    b = std::move(a);
    CHECK(b.size() == 3_u);
    CHECK(a.size() == 3_u);
  }

  {  // move construct
    using MoveOnly = std::unique_ptr<int>;
    stack_vector<MoveOnly, 3> a(3);
    CHECK(a.size() == 3_u);
    stack_vector<MoveOnly, 3> b(std::move(a));
    CHECK(b.size() == 3_u);
    CHECK(a.size() == 3_u);
  }

  return test::result();
}
