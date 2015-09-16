// Adapted from: https://github.com/akrzemi1/compact_optional
//
// Copyright (C) 2015, Andrzej Krzemienski.
//
// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <utility>
#include <string>
#include <ndtree/utility/compact_optional.hpp>
#include "../test.hpp"

using namespace ndtree;

template <typename T> void ignore(T&&) {}

void test_value_ctor() {
  {
    typedef compact_optional<empty_scalar_value<int, -1>> opt_int;
    static_assert(sizeof(opt_int) == sizeof(int), "size waste");

    opt_int oi_, oi0(0), oi1(1);
    CHECK(!oi_.has_value());
    CHECK(oi0.has_value());
    CHECK(oi1.has_value());

    CHECK(oi0.value() == 0);
    CHECK(oi1.value() == 1);
  }
  {
    typedef compact_optional<empty_scalar_value<int, 0>> opt_int;
    static_assert(sizeof(opt_int) == sizeof(int), "size waste");
    opt_int oi_, oi_n1(-1), oi_1(1);
    CHECK(!oi_.has_value());
    CHECK(oi_n1.has_value());
    CHECK(oi_1.has_value());

    CHECK(oi_n1.value() == -1);
    CHECK(oi_1.value() == 1);
  }
}

struct string_empty_value : compact_optional_type<std::string> {
  static std::string empty_value() { return std::string("\0\0", 2); }
  static bool is_empty_value(const std::string& v) {
    return v == std::string("\0\0", 2);
  }
};

void test_string_traits() {
  typedef compact_optional<string_empty_value, class tag_X> opt_str;
  static_assert(sizeof(opt_str) == sizeof(std::string), "size waste");

  opt_str os_, os_0(std::string("\0")), os_a(std::string("A"));
  CHECK(!os_.has_value());
  CHECK(os_0.has_value());
  CHECK(os_a.has_value());
}

struct string_in_pair_empty_val
 : compact_optional_type<std::string, std::pair<bool, std::string>> {
  static storage_type empty_value() { return storage_type(false, "anything"); }
  static bool is_empty_value(const storage_type& v) { return !v.first; }

  static const value_type& access_value(const storage_type& v) {
    return v.second;
  }
  static storage_type store_value(const value_type& v) {
    return storage_type(true, v);
  }
  static storage_type store_value(value_type&& v) {
    return storage_type(true, std::move(v));
  }
};

void test_custom_storage() {
  typedef compact_optional<string_in_pair_empty_val> opt_str;
  opt_str os_, os_0(std::string("\0")), os_a(std::string("A"));

  CHECK(!os_.has_value());

  CHECK(os_0.has_value());
  CHECK(os_0.value() == "");

  CHECK(os_a.has_value());
  CHECK(os_a.value() == "A");

  swap(os_, os_0);

  CHECK(os_.has_value());
  CHECK(os_.value() == "");
  CHECK(!os_0.has_value());
}

void test_bool_storage() {
  typedef compact_optional<compact_bool> opt_bool;
  static_assert(sizeof(opt_bool) == 1, "size waste");

  opt_bool ob_, ob_t(true), ob_f(false);
  CHECK(!ob_.has_value());

  CHECK(ob_t.has_value());
  CHECK(ob_t.value() == true);

  CHECK(ob_f.has_value());
  CHECK(ob_f.value() == false);
}

int main() {
  test_value_ctor();
  test_string_traits();
  test_custom_storage();
  test_bool_storage();

  return test::result();
}
