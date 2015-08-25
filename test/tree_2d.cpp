#include <fstream>
#include "test.hpp"
//#define NDTREE_TEST_PRIVATE_AS_PUBLIC
#include "test_tree.hpp"
#include <ndtree/algorithm/dfs_sort.hpp>
#include <ndtree/algorithm/node_location.hpp>

using namespace test;

/// Explicit instantiate it
template struct ndtree::tree<2>;

NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(
 std::declval<tree<2>>().children(node_idx{}));
NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(std::declval<tree<2>>()());

NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(tree<2>::child_positions());

struct uniform_tree {
  std::vector<node> nodes{
   {n(0, 0, i, cs{1, 2, 3, 4}, pip{}, {i, i, i, i})},
   {n(1, 1, 0, cs{5, 6, 7, 8}, pip{0}, {i, 2, i, 3})},      //
   {n(2, 1, 0, cs{9, 10, 11, 12}, pip{1}, {1, i, i, 4})},   //
   {n(3, 1, 0, cs{13, 14, 15, 16}, pip{2}, {i, 4, 1, i})},  //
   {n(4, 1, 0, cs{17, 18, 19, 20}, pip{3}, {3, i, 2, i})}   //
   //
  };
};

struct tree_after_refine {
  std::vector<node> nodes{
   {n(8, 2, 1, cs{21, 22, 23, 24}, pip{0, 3}, {7, 11, 6, 14},
      {5, 6, 7, 9, 11, 13, 14, 25})},  //
   {n(17, 2, 4, cs{25, 26, 27, 28}, pip{3, 0}, {14, 18, 11, 19},
      {11, 12, 14, 16, 18, 19, 20, 24})},  //
   // children of 8:
   {n(21, 3, 8, cs{}, pip{0, 3, 0}, {i, 22, i, 23})},  //
   {n(22, 3, 8, cs{}, pip{0, 3, 1}, {21, i, i, 24})},  //
   {n(23, 3, 8, cs{}, pip{0, 3, 2}, {i, 24, 21, i})},  //
   {n(24, 3, 8, cs{}, pip{0, 3, 3}, {23, i, 22, i},
      {11, 14, 21, 22, 23, 25})},  //
   // children of 17:
   {n(25, 3, 17, cs{}, pip{3, 0, 0}, {i, 26, i, 27},
      {11, 14, 24, 26, 27, 28})},                       //
   {n(26, 3, 17, cs{}, pip{3, 0, 1}, {25, i, i, 28})},  //
   {n(27, 3, 17, cs{}, pip{3, 0, 2}, {i, 28, 25, i})},  //
   {n(28, 3, 17, cs{}, pip{3, 0, 3}, {27, i, 26, i})}   //
  };
};
int main() {
  {  // check construction
    tree<2> t(1);
    CHECK(t.capacity() == 1_u);
    CHECK(t.size() == 1_u);
    CHECK(!t.empty());
    CHECK(t.is_leaf(0_n));
  }

  {  // check capacity
    CHECK(tree<2>(1).capacity() == 1_u);
    CHECK(tree<2>(2).capacity() == 5_u);
    CHECK(tree<2>(3).capacity() == 5_u);
    CHECK(tree<2>(4).capacity() == 5_u);
    CHECK(tree<2>(5).capacity() == 5_u);
    CHECK(tree<2>(6).capacity() == 9_u);
    CHECK(tree<2>(7).capacity() == 9_u);
    CHECK(tree<2>(8).capacity() == 9_u);
    CHECK(tree<2>(9).capacity() == 9_u);
    CHECK(tree<2>(10).capacity() == 13_u);
    CHECK(tree<2>(11).capacity() == 13_u);
    CHECK(tree<2>(12).capacity() == 13_u);
    CHECK(tree<2>(13).capacity() == 13_u);
  }
  {
    tree<2> t(29);
    CHECK(t.capacity() == 29_u);
    CHECK(t.size() == 1_u);
    CHECK(t.is_leaf(0_n));
    t.refine(0_n);
    CHECK(!t.is_leaf(0_n));
    CHECK(t.size() == 5_u);

    t.refine(1_n);
    t.refine(2_n);
    t.refine(3_n);
    t.refine(4_n);

    CHECK(t.size() == 21_u);
    check_all(t, uniform_tree{});

    t.refine(8_n);
    t.refine(17_n);

    CHECK(t.size() == 29_u);
    check_all(t, tree_after_refine{});
  }

  return test::result();
}
