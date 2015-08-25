#pragma once
/// \file test_tree.hpp

#ifdef NDTREE_TEST_PRIVATE_AS_PUBLIC
// Don't do this at home TM:
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wkeyword-macro"
#define private public
#include <ndtree/ndtree.hpp>
#pragma clang diagnostic pop
#else
#include <ndtree/tree.hpp>
#endif
#include <ndtree/algorithm/node_location.hpp>
#include <ndtree/algorithm/node_at.hpp>
#include <ndtree/algorithm/node_neighbor.hpp>
#include <ndtree/algorithm/node_neighbors.hpp>
#include <ndtree/relations/tree.hpp>

namespace test {

using namespace ndtree;

template <class Tag> struct tagged_initializer_list {
  template <class... Args>
  tagged_initializer_list(Args&&... args)
   : data{static_cast<uint_t>(args)...} {}
  std::vector<uint_t> data;
  auto begin() noexcept { return ndtree::begin(data); }
  auto begin() const noexcept { return ndtree::begin(data); }
  auto end() noexcept { return ndtree::end(data); }
  auto end() const noexcept { return ndtree::end(data); }
  auto size() const noexcept { return ndtree::size(data); }
};

/// Children
using cs = tagged_initializer_list<class children_tag_>;
/// Position in parent
using pip = tagged_initializer_list<class position_in_parent_tag_>;

struct node {
  node_idx idx;
  int level;
  node_idx parent;
  std::vector<node_idx> children;
  std::vector<uint_t> pos_in_parent;
  std::vector<node_idx> face_neighbors;
  std::vector<node_idx> all_neighbors;
};

static const constexpr auto i = std::numeric_limits<int>::max();

node n(uint_t idx, int level, int parent, cs children = {},
       pip pos_in_parent = {},
       std::initializer_list<uint_t> face_neighbors = {},
       std::initializer_list<uint_t> all_neighbors = {}) {
  node t;
  t.idx = node_idx{idx};
  t.level = level;
  t.parent = parent == i ? node_idx{} : node_idx{parent};
  t.children.resize(size(children));
  ranges::transform(children, begin(t.children),
                    [](int c) { return c != i ? node_idx{c} : node_idx{}; });
  t.pos_in_parent.resize(size(pos_in_parent));
  ranges::transform(pos_in_parent, begin(t.pos_in_parent),
                    [](int p) { return static_cast<uint_t>(p); });

  t.face_neighbors.resize(size(face_neighbors));
  ranges::transform(face_neighbors, begin(t.face_neighbors),
                    [](int c) { return c != i ? node_idx{c} : node_idx{}; });

  t.all_neighbors.resize(size(all_neighbors));
  ranges::transform(all_neighbors, begin(t.all_neighbors),
                    [](int c) { return c != i ? node_idx{c} : node_idx{}; });

  return t;
}

template <class Tree> void check_node(Tree const& t, node n) {
  constexpr int nd = Tree::dimension();
  if (n.parent) {
    CHECK(!t.is_root(n.idx));
  } else {
    CHECK(t.is_root(n.idx));
  }
  CHECK(t.parent(n.idx) == n.parent);
  CHECK(t.level(n.idx) == n.level);
  CHECK(t.no_children(n.idx) == (int)size(n.children));
  if (size(n.children) == 0_u) {
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
      CHECK(size(n.children) == 0_u);
    }
  }
  {  // check location: compute node location, find node at location, both must
     // be the same node:
    auto l = node_location(t, n.idx);
    auto nn = node_at(t, l);
    CHECK(nn == n.idx);
  }
  if (size(n.pos_in_parent) > 0_u) {
    test::check_equal(node_location(t, n.idx)(), n.pos_in_parent);
    CHECK(node_at(t, location<nd>(n.pos_in_parent)) == n.idx);
  }
  if (size(n.face_neighbors) > 0_u) {
    using neighbor_idx = neighbor_idx_t<face_neighbors<nd>>;
    for (auto&& p : neighbor_idx::rng()) {
      CHECK(node_neighbor(t, node_location(t, n.idx), p)
            == n.face_neighbors[*p]);
    }
  }
  if (size(n.all_neighbors) > 0_u) {
    auto neighbors = node_neighbors(t, node_location(t, n.idx));
    CHECK(size(neighbors) == size(n.all_neighbors));
    for (auto&& j : view::ints(0_u, size(neighbors))) {
      CHECK(neighbors[j] == n.all_neighbors[j]);
    }
  }
}

template <class Tree, class ReferenceTree>
void check_all(Tree const& t, ReferenceTree const& tref) {
  for (auto&& n : tref.nodes) { check_node(t, n); }
}

/// Tree to dot
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

}  // namespace test
