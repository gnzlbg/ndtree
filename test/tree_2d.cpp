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
   {idx{0}, lvl{0}, pn{i}, cs{1, 2, 3, 4}, pip{}, fn{i, i, i, i}},
   {idx{1}, lvl{1}, pn{0}, cs{5, 6, 7, 8}, pip{0}, fn{i, 2, i, 3}},      //
   {idx{2}, lvl{1}, pn{0}, cs{9, 10, 11, 12}, pip{1}, fn{1, i, i, 4}},   //
   {idx{3}, lvl{1}, pn{0}, cs{13, 14, 15, 16}, pip{2}, fn{i, 4, 1, i}},  //
   {idx{4}, lvl{1}, pn{0}, cs{17, 18, 19, 20}, pip{3}, fn{3, i, 2, i}}   //
                                                                         //
  };
};

struct tree_after_refine {
  std::vector<node> nodes{
   {idx{8}, lvl{2}, pn{1}, cs{21, 22, 23, 24}, pip{0, 3}, fn{7, 11, 6, 14},
    an{5, 6, 7, 9, 11, 13, 14, 25}},  //
   {idx{17}, lvl{2}, pn{4}, cs{25, 26, 27, 28}, pip{3, 0}, fn{14, 18, 11, 19},
    an{11, 12, 14, 16, 18, 19, 20, 24}},                            //
   {idx{21}, lvl{3}, pn{8}, cs{}, pip{0, 3, 0}, fn{i, 22, i, 23}},  //
   {idx{22}, lvl{3}, pn{8}, cs{}, pip{0, 3, 1}, fn{21, i, i, 24}},  //
   {idx{23}, lvl{3}, pn{8}, cs{}, pip{0, 3, 2}, fn{i, 24, 21, i}},  //
   {idx{24}, lvl{3}, pn{8}, cs{}, pip{0, 3, 3}, fn{23, i, 22, i},
    an{11, 14, 21, 22, 23, 25}},
   {idx{25}, lvl{3}, pn{17}, cs{}, pip{3, 0, 0}, fn{i, 26, i, 27},
    an{11, 14, 24, 26, 27, 28}},                                       //
   {idx{26}, lvl{3}, pn{17}, cs{}, pip{3, 0, 1}, fn{25, i, i, 28}},    //
   {idx{27}, lvl{3}, pn{17}, cs{}, pip{3, 0, 2}, fn{i, 28, 25, i}},    //
   {idx{28}, lvl{3}, pn{17}, cs{}, pip{3, 0, 3}, fn{27, i, 26, i}}  //
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
    check_tree(t, uniform_tree{});

    t.refine(8_n);
    t.refine(17_n);

    CHECK(t.size() == 29_u);
    check_tree(t, tree_after_refine{});
  }

  return test::result();
}
