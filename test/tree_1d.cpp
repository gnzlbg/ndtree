#include <fstream>
#include "test.hpp"
//#define NDTREE_TEST_PRIVATE_AS_PUBLIC
#include "test_tree.hpp"
#include <ndtree/algorithm/dfs_sort.hpp>
#include <ndtree/algorithm/node_location.hpp>

using namespace test;

/// Explicit instantiate it
template struct ndtree::tree<1>;

NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(
 std::declval<tree<1>>().children(node_idx{}));
NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(std::declval<tree<1>>()());

NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(tree<1>::child_positions());

struct uniform_tree {
  std::vector<node> nodes{
   {n(0, 0, i, {1, 2}, {})},        //
   {n(1, 1, 0, {3, 4}, {0})},       //
   {n(2, 1, 0, {5, 6}, {1})},       //
   {n(3, 2, 1, {7, 8}, {0, 0})},    //
   {n(4, 2, 1, {9, 10}, {0, 1})},   //
   {n(5, 2, 2, {11, 12}, {1, 0})},  //
   {n(6, 2, 2, {13, 14}, {1, 1})},  //
   {n(7, 3, 3, {}, {0, 0, 0})},     //
   {n(8, 3, 3, {}, {0, 0, 1})},     //
   {n(9, 3, 4, {}, {0, 1, 0})},     //
   {n(10, 3, 4, {}, {0, 1, 1})},    //
   {n(11, 3, 5, {}, {1, 0, 0})},    //
   {n(12, 3, 5, {}, {1, 0, 1})},    //
   {n(13, 3, 6, {}, {1, 1, 0})},    //
   {n(14, 3, 6, {}, {1, 1, 1})}     //
                                    //
  };
};

struct tree_after_refine {
  std::vector<node> nodes{
   {n(7, 3, 3, {}, {0, 0, 0})},         //
   {n(8, 3, 3, {17, 18}, {0, 0, 1})},   //
   {n(9, 3, 4, {19, 20}, {0, 1, 0})},   //
   {n(10, 3, 4, {}, {0, 1, 1})},        //
   {n(11, 3, 5, {15, 16}, {1, 0, 0})},  //
   {n(12, 3, 5, {}, {1, 0, 1})},        //
   {n(13, 3, 6, {}, {1, 1, 0})},        //
   {n(14, 3, 6, {}, {1, 1, 1})},        //
   {n(15, 4, 11, {}, {1, 0, 0, 0})},    //
   {n(16, 4, 11, {}, {1, 0, 0, 1})},    //
   {n(17, 4, 8, {}, {0, 0, 1, 0})},     //
   {n(18, 4, 8, {}, {0, 0, 1, 1})},     //
   {n(19, 4, 9, {}, {0, 1, 0, 0})},     //
   {n(20, 4, 9, {}, {0, 1, 0, 1})}      //
                                        //
  };
};

struct tree_after_coarsen {
  std::vector<node> nodes{
   {n(0, 0, i, {1, 2})},    //
   {n(1, 1, 0, {3, 4})},    //
   {n(2, 1, 0, {5, 6})},    //
   {n(3, 2, 1, {7, 8})},    //
   {n(4, 2, 1, {9, 10})},   //
   {n(5, 2, 2, {})},        //
   {n(6, 2, 2, {13, 14})},  //
   {n(7, 3, 3, {})},        //
   {n(8, 3, 3, {17, 18})},  //
   {n(9, 3, 4, {19, 20})},  //
   {n(10, 3, 4, {})},       //
   {n(13, 3, 6, {})},       //
   {n(14, 3, 6, {})},       //
   {n(17, 4, 8, {})},       //
   {n(18, 4, 8, {})},       //
   {n(19, 4, 9, {})},       //
   {n(20, 4, 9, {})}        //

   //
  };
};

struct tree_after_coarsen_sorted {
  std::vector<node> nodes{
   {n(0, 0, i, {1, 2})},     //
   {n(1, 1, 0, {3, 4})},     //
   {n(2, 1, 0, {13, 14})},   //
   {n(3, 2, 1, {5, 6})},     //
   {n(4, 2, 1, {9, 10})},    //
   {n(5, 3, 3, {})},         //
   {n(6, 3, 3, {7, 8})},     //
   {n(7, 4, 6, {})},         //
   {n(8, 4, 6, {})},         //
   {n(9, 3, 4, {11, 12})},   //
   {n(10, 3, 4, {})},        //
   {n(11, 4, 9, {})},        //
   {n(12, 4, 9, {})},        //
   {n(13, 2, 2, {})},        //
   {n(14, 2, 2, {15, 16})},  //
   {n(15, 3, 14, {})},       //
   {n(16, 3, 14, {})}        //

   //
  };
};

void check_construction() {
  {
    tree<1> t(1);
    CHECK(t.capacity() == 1_u);
    CHECK(t.size() == 1_u);
    CHECK(!t.empty());
    CHECK(t.is_leaf(0_n));
  }

  {
    CHECK(tree<1>(1).capacity() == 1_u);
    CHECK(tree<1>(2).capacity() == 3_u);
    CHECK(tree<1>(3).capacity() == 3_u);
    CHECK(tree<1>(4).capacity() == 5_u);
    CHECK(tree<1>(14).capacity() == 15_u);
    CHECK(tree<1>(15).capacity() == 15_u);
  }
}

int main() {
  check_construction();
  {
    tree<1> t(20);
    CHECK(t.capacity() == 21_u);
    CHECK(t.size() == 1_u);
    CHECK(t.is_leaf(0_n));
    t.refine(0_n);
    CHECK(!t.is_leaf(0_n));
    CHECK(t.size() == 3_u);

    {  // node locs
      auto l1 = node_location(t, 1_n);
      CHECK(l1.level == 1_u);
      CHECK(l1[1] == 0u);
      auto l2 = node_location(t, 2_n);
      CHECK(l2.level == 1_u);
      CHECK(l2[1] == 1u);
    }

    t.refine(1_n);
    CHECK(t.size() == 5_u);

    {  // node locs
      auto l3 = node_location(t, 3_n);
      CHECK(l3.level == 2_u);
      CHECK(l3[1] == 0_u);
      CHECK(l3[2] == 0_u);
      auto l4 = node_location(t, 4_n);
      CHECK(l4.level == 2_u);
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

    check_all(t, uniform_tree{});
    CHECK(t.is_compact());

    t.refine(11_n);
    CHECK(t.size() == 17_u);
    t.refine(8_n);
    CHECK(t.size() == 19_u);
    t.refine(9_n);
    CHECK(t.size() == 21_u);

    check_all(t, tree_after_refine{});
    CHECK(t.is_compact());

    t.coarsen(11_n);
    CHECK(t.size() == 19_u);
    CHECK(!t.is_compact());

    t.coarsen(5_n);
    CHECK(t.size() == 17_u);

    CHECK(!t.is_compact());
    check_all(t, tree_after_coarsen{});

    dfs_sort(t);
    CHECK(t.is_compact());
    check_all(t, tree_after_coarsen_sorted{});
  }

  return test::result();
}
