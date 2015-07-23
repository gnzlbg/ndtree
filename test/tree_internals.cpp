#include "test.hpp"
#include <fstream>

#define NDTREE_TEST_PRIVATE_AS_PUBLIC
#include "test_tree.hpp"

using namespace test;

NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(
 std::declval<tree<1>>().nodes(siblings_idx{}));

// static_assert(BidirectionalRange<decltype(std::declval<tree<1>>().nodes())>{},
//               "tree.nodes() is not a bidirectional range!");

int main() {
  using tree_t = tree<1>;

  static_assert(tree_t::sibling_group(0_n) == 0_sg, "");
  static_assert(tree_t::sibling_group(1_n) == 1_sg, "");
  static_assert(tree_t::sibling_group(2_n) == 1_sg, "");
  static_assert(tree_t::sibling_group(3_n) == 2_sg, "");
  static_assert(tree_t::sibling_group(4_n) == 2_sg, "");
  static_assert(tree_t::sibling_group(5_n) == 3_sg, "");
  static_assert(tree_t::sibling_group(6_n) == 3_sg, "");

  static_assert(tree_t::no_sibling_groups(0_n) == 0_sg, "");
  static_assert(tree_t::no_sibling_groups(1_n) == 1_sg, "");
  static_assert(tree_t::no_sibling_groups(2_n) == 2_sg, "");
  static_assert(tree_t::no_sibling_groups(3_n) == 2_sg, "");
  static_assert(tree_t::no_sibling_groups(4_n) == 3_sg, "");
  static_assert(tree_t::no_sibling_groups(5_n) == 3_sg, "");
  static_assert(tree_t::no_sibling_groups(6_n) == 4_sg, "");

  static_assert(tree_t::no_nodes(0_sg) == 0_n, "");
  static_assert(tree_t::no_nodes(1_sg) == 1_n, "");
  static_assert(tree_t::no_nodes(2_sg) == 3_n, "");
  static_assert(tree_t::no_nodes(3_sg) == 5_n, "");
  static_assert(tree_t::no_nodes(4_sg) == 7_n, "");
  static_assert(tree_t::no_nodes(5_sg) == 9_n, "");
  static_assert(tree_t::no_nodes(6_sg) == 11_n, "");

  // TODO:
  // check no sibling groups close to numeric_limits<uint_t>::max()
  // place asserts that check that we never overflow

  tree<1> t(20);
  t.refine(0_n);
  t.refine(1_n);

  struct test_ns {
    std::vector<node> nodes{
     {n(0, 0, i, {1, 2})},  //
     {n(1, 1, 0, {3, 4})},  //
     {n(2, 1, 0, {})},      //
     {n(3, 2, 1, {})},      //
     {n(4, 2, 1, {})}       //
                            //
    };
  };

  { check_all(t, test_ns{}); }

  t.swap(1_sg, 2_sg);
  {
    struct test_s {
      std::vector<node> nodes{
       {n(0, 0, i, {3, 4})},  //
       {n(1, 2, 3, {})},      //
       {n(2, 2, 3, {})},      //
       {n(3, 1, 0, {1, 2})},  //
       {n(4, 1, 0, {})}       //
                              //
      };
    };
    check_all(t, test_s{});
  }

  t.swap(1_sg, 2_sg);
  { check_all(t, test_ns{}); }

  return test::result();
};
