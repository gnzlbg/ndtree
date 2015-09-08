#include <fstream>
#include "test.hpp"
#include "tree.hpp"
#include <ndtree/algorithm/dfs_sort.hpp>
#include <ndtree/algorithm/node_location.hpp>
#include <ndtree/location/slim.hpp>

using namespace test;

/// Explicit instantiate it
template struct ndtree::tree<1>;

NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(
 std::declval<tree<1>>().children(node_idx{}));
NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(std::declval<tree<1>>()());

NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(tree<1>::child_positions());

struct uniform_tree {
  std::vector<node> nodes{
   {idx{0}, lvl{0}, pn{i}, cs{1, 2}, pip{}, fn{i, i}, en{}, cn{}, an{},
    nc{0.5}},                                                                //
   {idx{1}, lvl{1}, pn{0}, cs{3, 4}, pip{0}, fn{i, 2}, an{5}, nc{0.25}},     //
   {idx{2}, lvl{1}, pn{0}, cs{5, 6}, pip{1}, fn{1, i}, an{4}, nc{0.75}},     //
   {idx{3}, lvl{2}, pn{1}, cs{7, 8}, pip{0, 0}, fn{i, 4}, an{9}, nc{.125}},  //
   {idx{4}, lvl{2}, pn{1}, cs{9, 10}, pip{0, 1}, fn{3, 5}, an{8, 11},
    nc{.375}},  //
   {idx{5}, lvl{2}, pn{2}, cs{11, 12}, pip{1, 0}, fn{4, 6}, an{10, 13},
    nc{0.625}},  //
   {idx{6}, lvl{2}, pn{2}, cs{13, 14}, pip{1, 1}, fn{5, i}, an{12},
    nc{0.875}},                                                           //
   {idx{7}, lvl{3}, pn{3}, cs{}, pip{0, 0, 0}, fn{i, 8}, an{8}},          //
   {idx{8}, lvl{3}, pn{3}, cs{}, pip{0, 0, 1}, fn{7, 9}, an{7, 9}},       //
   {idx{9}, lvl{3}, pn{4}, cs{}, pip{0, 1, 0}, fn{8, 10}, an{8, 10}},     //
   {idx{10}, lvl{3}, pn{4}, cs{}, pip{0, 1, 1}, fn{9, 11}, an{9, 11}},    //
   {idx{11}, lvl{3}, pn{5}, cs{}, pip{1, 0, 0}, fn{10, 12}, an{10, 12}},  //
   {idx{12}, lvl{3}, pn{5}, cs{}, pip{1, 0, 1}, fn{11, 13}, an{11, 13}},  //
   {idx{13}, lvl{3}, pn{6}, cs{}, pip{1, 1, 0}, fn{12, 14}, an{12, 14}},  //
   {idx{14}, lvl{3}, pn{6}, cs{}, pip{1, 1, 1}, fn{13, i}, an{13}}        //
                                                                          //
  };
};

struct tree_after_refine {
  std::vector<node> nodes{
   {idx{7}, lvl{3}, pn{3}, cs{}, pip{0, 0, 0}, fn{i, 8}, an{17}},             //
   {idx{8}, lvl{3}, pn{3}, cs{17, 18}, pip{0, 0, 1}, fn{7, 9}, an{7, 19}},    //
   {idx{9}, lvl{3}, pn{4}, cs{19, 20}, pip{0, 1, 0}, fn{8, 10}, an{10, 18}},  //
   {idx{10}, lvl{3}, pn{4}, cs{}, pip{0, 1, 1}, fn{9, 11}, an{15, 20}},       //
   {idx{11}, lvl{3}, pn{5}, cs{15, 16}, pip{1, 0, 0}, fn{10, 12},
    an{10, 12}},                                                             //
   {idx{12}, lvl{3}, pn{5}, cs{}, pip{1, 0, 1}, fn{11, 13}, an{13, 16}},     //
   {idx{13}, lvl{3}, pn{6}, cs{}, pip{1, 1, 0}, fn{12, 14}, an{12, 14}},     //
   {idx{14}, lvl{3}, pn{6}, cs{}, pip{1, 1, 1}, fn{13, i}, an{13}},          //
   {idx{15}, lvl{4}, pn{11}, cs{}, pip{1, 0, 0, 0}, fn{i, 16}, an{10, 16}},  //
   {idx{16}, lvl{4}, pn{11}, cs{}, pip{1, 0, 0, 1}, fn{15, i}, an{12, 15}},  //
   {idx{17}, lvl{4}, pn{8}, cs{}, pip{0, 0, 1, 0}, fn{i, 18}, an{7, 18}},    //
   {idx{18}, lvl{4}, pn{8}, cs{}, pip{0, 0, 1, 1}, fn{17, 19}, an{17, 19}},  //
   {idx{19}, lvl{4}, pn{9}, cs{}, pip{0, 1, 0, 0}, fn{18, 20}, an{18, 20}},  //
   {idx{20}, lvl{4}, pn{9}, cs{}, pip{0, 1, 0, 1}, fn{19, i}, an{10, 19}}    //
                                                                             //
  };
};

struct tree_after_coarsen {
  std::vector<node> nodes{
   {idx{0}, lvl{0}, pn{i}, cs{1, 2}, fn{i, i}, an{}},           //
   {idx{1}, lvl{1}, pn{0}, cs{3, 4}, fn{i, 2}, an{5}},          //
   {idx{2}, lvl{1}, pn{0}, cs{5, 6}, fn{1, i}, an{4}},          //
   {idx{3}, lvl{2}, pn{1}, cs{7, 8}, fn{i, 4}, an{9}},          //
   {idx{4}, lvl{2}, pn{1}, cs{9, 10}, fn{3, 5}, an{5, 8}},      //
   {idx{5}, lvl{2}, pn{2}, cs{}, fn{4, 6}, an{10, 13}},         //
   {idx{6}, lvl{2}, pn{2}, cs{13, 14}, fn{5, i}, an{5}},        //
   {idx{7}, lvl{3}, pn{3}, cs{}, fn{i, 8}, an{17}},             //
   {idx{8}, lvl{3}, pn{3}, cs{17, 18}, fn{7, 9}, an{7, 19}},    //
   {idx{9}, lvl{3}, pn{4}, cs{19, 20}, fn{8, 10}, an{10, 18}},  //
   {idx{10}, lvl{3}, pn{4}, cs{}, fn{9, i}, an{5, 20}},         //
   {idx{13}, lvl{3}, pn{6}, cs{}, fn{i, 14}, an{5, 14}},        //
   {idx{14}, lvl{3}, pn{6}, cs{}, fn{13, i}, an{13}},           //
   {idx{17}, lvl{4}, pn{8}, cs{}, fn{i, 18}, an{7, 18}},        //
   {idx{18}, lvl{4}, pn{8}, cs{}, fn{17, 19}, an{17, 19}},      //
   {idx{19}, lvl{4}, pn{9}, cs{}, fn{18, 20}, an{18, 20}},      //
   {idx{20}, lvl{4}, pn{9}, cs{}, fn{19, i}, an{10, 19}}        //

   //
  };
};

struct tree_after_coarsen_sorted {
  std::vector<node> nodes{
   {idx{0}, lvl{0}, pn{i}, cs{1, 2}},     //
   {idx{1}, lvl{1}, pn{0}, cs{3, 4}},     //
   {idx{2}, lvl{1}, pn{0}, cs{13, 14}},   //
   {idx{3}, lvl{2}, pn{1}, cs{5, 6}},     //
   {idx{4}, lvl{2}, pn{1}, cs{9, 10}},    //
   {idx{5}, lvl{3}, pn{3}, cs{}},         //
   {idx{6}, lvl{3}, pn{3}, cs{7, 8}},     //
   {idx{7}, lvl{4}, pn{6}, cs{}},         //
   {idx{8}, lvl{4}, pn{6}, cs{}},         //
   {idx{9}, lvl{3}, pn{4}, cs{11, 12}},   //
   {idx{10}, lvl{3}, pn{4}, cs{}},        //
   {idx{11}, lvl{4}, pn{9}, cs{}},        //
   {idx{12}, lvl{4}, pn{9}, cs{}},        //
   {idx{13}, lvl{2}, pn{2}, cs{}},        //
   {idx{14}, lvl{2}, pn{2}, cs{15, 16}},  //
   {idx{15}, lvl{3}, pn{14}, cs{}},       //
   {idx{16}, lvl{3}, pn{14}, cs{}}        //

   //
  };
};

template <template <ndtree::uint_t, class...> class Loc> void test_tree() {
  {  // check construction
    tree<1> t(1);
    CHECK(t.capacity() == 1_u);
    CHECK(t.size() == 1_u);
    CHECK(!t.empty());
    CHECK(t.is_leaf(0_n));
  }

  {  // check capacity
    CHECK(tree<1>(1).capacity() == 1_u);
    CHECK(tree<1>(2).capacity() == 3_u);
    CHECK(tree<1>(3).capacity() == 3_u);
    CHECK(tree<1>(4).capacity() == 5_u);
    CHECK(tree<1>(14).capacity() == 15_u);
    CHECK(tree<1>(15).capacity() == 15_u);
  }
  {
    tree<1> t(20);
    CHECK(t.capacity() == 21_u);
    CHECK(t.size() == 1_u);
    CHECK(t.is_leaf(0_n));
    t.refine(0_n);
    CHECK(!t.is_leaf(0_n));
    CHECK(t.size() == 3_u);

    {  // node locs
      auto l1 = node_location(t, 1_n, Loc<1>{});
      CHECK(l1.level() == 1_u);
      CHECK(l1[1] == 0u);
      auto l2 = node_location(t, 2_n, Loc<1>{});
      CHECK(l2.level() == 1_u);
      CHECK(l2[1] == 1u);
    }

    t.refine(1_n);
    CHECK(t.size() == 5_u);

    {  // node locs
      auto l3 = node_location(t, 3_n, Loc<1>{});
      CHECK(l3.level() == 2_u);
      CHECK(l3[1] == 0_u);
      CHECK(l3[2] == 0_u);
      auto l4 = node_location(t, 4_n, Loc<1>{});
      CHECK(l4.level() == 2_u);
      CHECK(l4[1] == 0_u);
      CHECK(l4[2] == 1_u);
    }

    t.refine(2_n);
    CHECK(t.size() == 7_u);

    t.refine(3_n);
    CHECK(t.size() == 9_u);
    t.refine(4_n);
    CHECK(t.size() == 11_u);
    t.refine(5_n);
    CHECK(t.size() == 13_u);
    t.refine(6_n);
    CHECK(t.size() == 15_u);

    check_tree(t, uniform_tree{}, Loc<1>{});
    CHECK(t.is_compact());

    t.refine(11_n);
    CHECK(t.size() == 17_u);
    t.refine(8_n);
    CHECK(t.size() == 19_u);
    t.refine(9_n);
    CHECK(t.size() == 21_u);

    check_tree(t, tree_after_refine{}, Loc<1>{});
    CHECK(t.is_compact());

    t.coarsen(11_n);
    CHECK(t.size() == 19_u);
    CHECK(!t.is_compact());

    t.coarsen(5_n);
    CHECK(t.size() == 17_u);

    CHECK(!t.is_compact());
    check_tree(t, tree_after_coarsen{}, Loc<1>{});

    auto t2 = t;
    CHECK(t == t2);
    CHECK(!(t != t2));

    dfs_sort(t);
    CHECK(t != t2);
    CHECK(!(t == t2));
    CHECK(t.is_compact());
    check_tree(t, tree_after_coarsen_sorted{}, Loc<1>{});
  }
}

int main() {
  test_tree<location::fast>();
  test_tree<location::slim>();
  return test::result();
}
