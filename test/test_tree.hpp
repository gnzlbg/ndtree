#pragma once
/// \file test_tree.hpp

#ifdef NDTREE_TEST_PRIVATE_AS_PUBLIC
// I know what I'm doing TM:
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wkeyword-macro"
#define private public
#include <ndtree/ndtree.hpp>
#pragma clang diagnostic pop
#else
#include <ndtree/tree.hpp>
#endif
#include <ndtree/algorithm/node_location.hpp>
#include <ndtree/algorithm/find_node.hpp>

struct test_node {
  ndtree::node_idx idx;
  int level;
  ndtree::node_idx parent;
  std::vector<ndtree::node_idx> children;
  std::vector<ndtree::uint_t> pos_in_parent;
};

static const constexpr auto i = std::numeric_limits<int>::max();

test_node n(int idx, int level, int parent, std::initializer_list<int> children,
            std::initializer_list<int> pos_in_parent = {}) {
  test_node t;
  t.idx = ndtree::node_idx{idx};
  t.level = level;
  t.parent = parent == i ? ndtree::node_idx{} : ndtree::node_idx{parent};
  t.children.resize(children.size());
  ranges::transform(children, begin(t.children),
                    [](int c) { return ndtree::node_idx{c}; });
  t.pos_in_parent.resize(pos_in_parent.size());
  ranges::transform(pos_in_parent, begin(t.pos_in_parent),
                    [](int p) { return static_cast<ndtree::uint_t>(p); });

  return t;
}

template <int nd> void check_node(ndtree::tree<nd>& t, test_node n) {
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
  if (n.pos_in_parent.size() > 0) {
    test::check_equal(ndtree::node_location(t, n.idx)(), n.pos_in_parent);
    CHECK(ndtree::find_node(t, ndtree::location<nd>(n.pos_in_parent)) == n.idx);
  }
}

template <class ReferenceTree>
void check_all(ndtree::tree<1>& t, ReferenceTree const& tref) {
  for (auto&& n : tref.nodes) { check_node(t, n); }
}

template <class Ostream, class Tree>
void pretty_print(Ostream&& os, Tree const& t) {
  using namespace ndtree;
  using std::to_string;
  os << "digraph graphname {\n";
  os << "concentrate=true\n";

  const auto sibling_group_label
   = [](const siblings_idx sg) { return "g" + to_string(*sg); };

  const auto sibling_label
   = [](const node_idx s) { return "s" + to_string(*s); };

  const auto node_label = [=](const siblings_idx sg, const node_idx nIdx) {
    return sibling_group_label(sg) + ":" + sibling_label(nIdx);
  };

  // write sibling groups:
  const auto last = t.no_children();
  RANGES_FOR(auto s, t.sibling_groups()) {
    os << sibling_group_label(s);
    os << "[label=\"";
    os << "<gg" << *s << "> " << to_string(*s) << "* |";
    auto c = 1;
    RANGES_FOR(auto&& n, t.nodes(s)) {
      os << "<" << sibling_label(n) << "> " << to_string(*n);
      if (n != 0_n and c != last) { os << "|"; }
      c++;
    };
    os << "\", shape=record];\n";
  };

  // write links from parent->children
  RANGES_FOR(auto&& s, t.sibling_groups()) {
    RANGES_FOR(auto&& n, t.nodes(s)) {
      for (auto&& c : t.children(n)) {
        if (c) {
          os << node_label(s, n) << " -> " << node_label(t.sibling_group(c), c)
             << ";\n";
        }
      }
    }
  }

  /// write links from sibling group - >parent
  for (auto&& s : t.sibling_groups()) {
    if (s == 0_sg) { continue; }
    const auto p = t.parent(s);
    const auto p_sg = t.sibling_group(p);
    os << sibling_group_label(s) << ":g" << sibling_group_label(s) << " -> "
       << node_label(p_sg, p) << ";\n";
  }

  os << "}" << std::endl;
}
