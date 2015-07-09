#include <htree/htree.hpp>
#include "test.hpp"

using namespace htree;

/// Explicit instantiate it
template struct htree::tree<1>;

HTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(
 std::declval<tree<1>>().children(node_idx{}));
HTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(std::declval<tree<1>>()());

HTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(tree<1>::child_positions());

struct test_node {
  node_idx idx;
  int level;
  node_idx parent;
  std::vector<node_idx> children;
};

static const constexpr auto i = std::numeric_limits<int>::max();

test_node n(int idx, int level, int parent,
            std::initializer_list<int> children) {
  test_node t;
  t.idx = node_idx{idx};
  t.level = level;
  t.parent = node_idx{parent};
  t.children.resize(children.size());
  transform(children, begin(t.children), [](int c) { return node_idx{c}; });
  return t;
}

void check_node(tree<1>& t, test_node n) {
  if (n.parent) {
    CHECK(!t.is_root(n.idx));
  } else {
    CHECK(t.is_root(n.idx));
  }
  CHECK(t.parent(n.idx) == n.parent);
  CHECK(t.level(n.idx) == n.level);
  CHECK(t.no_children(n.idx) == (int)n.children.size());
  if (n.children.size() == 0) {
    CHECK(t.is_leaf(n.idx));
  } else {
    CHECK(!t.is_leaf(n.idx));
  }
  test::check_equal(t.children(n.idx), n.children);
  for (auto p : t.child_positions()) {
    auto c = t.child(n.idx, p);
    if (c) {
      CHECK(c == n.children[*p]);
    } else {
      CHECK(n.children.size() == 0ul);
    }
  }
}

struct uniform_tree {
  std::vector<test_node> nodes{
   {n(0, 0, i, {1, 2})},    //
   {n(1, 1, 0, {3, 4})},    //
   {n(2, 1, 0, {5, 6})},    //
   {n(3, 2, 1, {7, 8})},    //
   {n(4, 2, 1, {9, 10})},   //
   {n(5, 2, 2, {11, 12})},  //
   {n(6, 2, 2, {13, 14})},  //
   {n(7, 3, 3, {})},        //
   {n(8, 3, 3, {})},        //
   {n(9, 3, 4, {})},        //
   {n(10, 3, 4, {})},       //
   {n(11, 3, 5, {})},       //
   {n(12, 3, 5, {})},       //
   {n(13, 3, 6, {})},       //
   {n(14, 3, 6, {})}        //
                            //
  };
};

struct tree_after_refine {
  std::vector<test_node> nodes{
   {n(7, 3, 3, {})},         //
   {n(8, 3, 3, {17, 18})},   //
   {n(9, 3, 4, {19, 20})},   //
   {n(10, 3, 4, {})},        //
   {n(11, 3, 5, {15, 16})},  //
   {n(12, 3, 5, {})},        //
   {n(13, 3, 6, {})},        //
   {n(14, 3, 6, {})},        //
   {n(15, 4, 11, {})},       //
   {n(16, 4, 11, {})},       //
   {n(17, 4, 8, {})},        //
   {n(18, 4, 8, {})},        //
   {n(19, 4, 9, {})},        //
   {n(20, 4, 9, {})}         //
                             //
  };
};

struct tree_after_coarsen {
  std::vector<test_node> nodes{
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
   {n(14, 3, 6, {})}        //
                            //
  };
};

template <class ReferenceTree>
void check_all(tree<1>& t, ReferenceTree const& tref) {
  for (auto&& n : tref.nodes) { check_node(t, n); }
}

void check_construction() {
  {
    tree<1> t(1);
    CHECK(t.capacity() == 1ul);
    CHECK(t.size() == 1ul);
    CHECK(!t.empty());
    CHECK(t.is_leaf(0_n));
  }

  {
    CHECK(tree<1>(1).capacity() == 1ul);
    CHECK(tree<1>(2).capacity() == 3ul);
    CHECK(tree<1>(3).capacity() == 3ul);
    CHECK(tree<1>(4).capacity() == 5ul);
    CHECK(tree<1>(14).capacity() == 15ul);
    CHECK(tree<1>(15).capacity() == 15ul);
  }
}

int main() {
  check_construction();
  {
    tree<1> t(20);
    CHECK(t.capacity() == 21ul);
    CHECK(t.size() == 1ul);
    CHECK(t.is_leaf(0_n));
    t.refine(0_n);
    CHECK(!t.is_leaf(0_n));
    CHECK(t.size() == 3ul);

    t.refine(1_n);
    CHECK(t.size() == 5ul);
    t.refine(2_n);
    CHECK(t.size() == 7ul);

    t.refine(3_n);
    CHECK(t.size() == 9ul);
    t.refine(4_n);
    CHECK(t.size() == 11ul);
    t.refine(5_n);
    CHECK(t.size() == 13ul);
    t.refine(6_n);
    CHECK(t.size() == 15ul);

    check_all(t, uniform_tree{});
    CHECK(t.is_compact());

    t.refine(11_n);
    CHECK(t.size() == 17ul);
    t.refine(8_n);
    CHECK(t.size() == 19ul);
    t.refine(9_n);
    CHECK(t.size() == 21ul);

    check_all(t, tree_after_refine{});
    CHECK(t.is_compact());

    t.coarsen(11_n);
    CHECK(t.size() == 19ul);

    t.coarsen(5_n);
    CHECK(t.size() == 17ul);

    check_all(t, tree_after_coarsen{});
    CHECK(!t.is_compact());
  }

  return test::result();
}
