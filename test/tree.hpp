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
#include <ndtree/algorithm.hpp>
#include <ndtree/location/fast.hpp>
#include <ndtree/relations/tree.hpp>
#include <ndtree/utility/optional.hpp>

namespace test {

using namespace ndtree;
using ndtree::optional;

/// invalid value
static const constexpr auto i = std::numeric_limits<int>::max();

/// Testing utility for constructing a node with named argument lists
template <typename Tag, typename T = uint_t> struct tagged_initializer_list {
  template <typename... Args>
  tagged_initializer_list(Args&&... args)
   : data{static_cast<T>(args)...} {}
  std::vector<T> data;
  auto begin() noexcept { return ndtree::begin(data); }
  auto begin() const noexcept { return ndtree::begin(data); }
  auto end() noexcept { return ndtree::end(data); }
  auto end() const noexcept { return ndtree::end(data); }
  auto size() const noexcept { return ndtree::size(data); }

  T operator*() const noexcept {
    if (ranges::size(data) != 1_u) {
      NDTREE_TERMINATE(
       "cannot dereference tagged_initializer_list of size {} != 1",
       ranges::size(data));
    };
    return at(data, 0);
  }
};

/// Node Index
using idx = tagged_initializer_list<class node_idx_tag_>;
/// Node Level
using lvl = tagged_initializer_list<class level_tag_>;
/// Node's parent node
using pn = tagged_initializer_list<class parent_node_tag_>;
/// Node's children
using cs = tagged_initializer_list<class children_tag_>;
/// Position in parent from root to node
using pip = tagged_initializer_list<class position_in_parent_tag_>;
/// Face neighbors
using fn = tagged_initializer_list<class face_neighbors_tag_>;
/// Edge neighbors
using en = tagged_initializer_list<class edge_neighbors_tag_>;
/// Corner neighbors
using cn = tagged_initializer_list<class corner_neighbors_tag_>;
/// All neighbors
using an = tagged_initializer_list<class all_neighbors_tag_>;
/// Normalized coordinates
using nc = tagged_initializer_list<class normalized_coordinates_tag_, num_t>;

/// Test data for a single node
struct node {
  optional<node_idx> idx{};
  optional<uint_t> level{};
  optional<node_idx> parent{};
  optional<std::vector<node_idx>> children{};
  optional<std::vector<uint_t>> pos_in_parent{};
  optional<std::vector<node_idx>> face_neighbors{};
  optional<std::vector<node_idx>> edge_neighbors{};
  optional<std::vector<node_idx>> corner_neighbors{};
  optional<std::vector<node_idx>> all_neighbors{};
  optional<std::vector<num_t>> normalized_coordinates{};

  node() = default;

  template <typename Arg, typename... Args>
  void init(Arg&& arg, Args&&... args) {
    init(std::forward<Arg>(arg));
    init(std::forward<Args>(args)...);
  }

  template <typename... Args> node(Args&&... args) {
    init(std::forward<Args>(args)...);
  }

  void init(test::idx j) { idx = node_idx{*j}; }
  void init(lvl l) { level = *l; }
  void init(pn p) { parent = *p != i ? node_idx{*p} : node_idx{}; }
  void init(cs c) {
    children = std::vector<node_idx>(size(c));
    ranges::transform(c, begin(*children),
                      [](int j) { return j != i ? node_idx{j} : node_idx{}; });
  }
  void init(pip ps) {
    pos_in_parent = std::vector<uint_t>(size(ps));
    ranges::transform(ps, begin(*pos_in_parent),
                      [](int p) { return static_cast<uint_t>(p); });
  }
  void init(fn ns) {
    face_neighbors = std::vector<node_idx>(size(ns));
    ranges::transform(ns, begin(*face_neighbors),
                      [](int c) { return c != i ? node_idx{c} : node_idx{}; });
  }
  void init(en ns) {
    edge_neighbors = std::vector<node_idx>(size(ns));
    ranges::transform(ns, begin(*edge_neighbors),
                      [](int c) { return c != i ? node_idx{c} : node_idx{}; });
  }
  void init(cn ns) {
    corner_neighbors = std::vector<node_idx>(size(ns));
    ranges::transform(ns, begin(*corner_neighbors),
                      [](int c) { return c != i ? node_idx{c} : node_idx{}; });
  }
  void init(an ns) {
    all_neighbors = std::vector<node_idx>(size(ns));
    ranges::transform(ns, begin(*all_neighbors),
                      [](int c) { return c != i ? node_idx{c} : node_idx{}; });
  }
  void init(nc x) {
    normalized_coordinates = std::vector<num_t>(size(x));
    ranges::copy(x, begin(*normalized_coordinates));
  }
};

template <typename Tree> void test_parent(Tree const& t, node const& n) {
  // consistency:
  if (t.parent(*n.idx)) {
    CHECK(any_of(t.children(t.parent(*n.idx)),
                 [&](auto&& c) { return c == *n.idx; }));
    CHECK(!t.is_root(*n.idx));
  } else {
    CHECK(t.is_root(*n.idx));
  }
  // check parent node:
  if (n.parent) { CHECK(t.parent(*n.idx) == *n.parent); }
}

template <typename Tree> void test_level(Tree const& t, node const& n) {
  // consistency:
  if (node_level(t, *n.idx) == 0) {
    CHECK(t.is_root(*n.idx));
  } else {
    CHECK(!t.is_root(*n.idx));
  }
  // check node level:
  if (n.level) { CHECK(node_level(t, *n.idx) == *n.level); }
}
template <typename Tree> void test_children(Tree const& t, node const& n) {
  // consistency
  if (t.no_children(*n.idx) == 0_u) {
    CHECK(t.is_leaf(*n.idx));
  } else {
    CHECK(!t.is_leaf(*n.idx));
    for (auto p : t.child_positions()) {
      auto c = t.child(*n.idx, p);
      if (c) { CHECK(t.parent(c) == *n.idx); }
    }
  }

  if (n.children) {
    CHECK(t.no_children(*n.idx) == size(*n.children));
    test::check_equal(t.children(*n.idx), *n.children);
    for (auto p : t.child_positions()) {
      auto c = t.child(*n.idx, p);
      if (c) {
        CHECK(c == (*n.children)[*p]);
      } else {
        CHECK(size(*n.children) == 0_u);
      }
    }
  }
}

template <typename Tree, typename Location>
void test_node_location(Tree const& t, node const& n, Location l) {
  static_assert(Tree::dimension() == Location::dimension(), "");
  // consistency:
  CHECK(node_at(t, node_location(t, *n.idx, l)) == *n.idx);
}

template <typename Tree, typename Location>
void test_pos_in_parent(Tree const& t, node const& n, Location l) {
  static_assert(Tree::dimension() == Location::dimension(), "");
  if (!n.pos_in_parent) { return; }
  test::check_equal(node_location(t, *n.idx, l)(), *n.pos_in_parent);
  CHECK(node_at(t, Location(*n.pos_in_parent)) == *n.idx);
}

template <typename Tree, typename Neighbors, typename NeighborType,
          typename Location>
void test_node_neighbor(Tree const& t, node const& n, Neighbors const& ns,
                        NeighborType, Location l) {
  if (!ns) { return; }
  using neighbor_idx = neighbor_idx_t<NeighborType>;
  CHECK(size(neighbor_idx::rng()) == size(*ns));
  for (auto&& p : neighbor_idx::rng()) {
    auto neighbor = node_neighbor(t, node_location(t, *n.idx, l), p);
    CHECK(neighbor == (*ns)[*p]);
    if (neighbor) {
      CHECK(node_neighbor(t, node_location(t, neighbor, l), opposite(p))
            == *n.idx);
    }
  }
}

template <typename Tree, typename Neighbors, typename Location>
void test_node_neighbors(Tree const& t, node const& n, Neighbors const& ns,
                         Location l) {
  if (!ns) { return; }
  auto neighbors = node_neighbors(t, node_location(t, *n.idx, l));
  CHECK(size(neighbors) == size(*ns));
  test::check_equal(neighbors, *ns);
}

template <typename Tree, typename Location>
void test_normalized_coordinates(Tree const& t, node const& n, Location l) {
  auto nc = normalized_coordinates(t, *n.idx, l);
  if (n.normalized_coordinates) {
    auto reference = *n.normalized_coordinates;
    test::check_equal(nc, reference);
  }
}

template <typename Tree, typename Location>
void check_node(Tree const& t, node n, Location l) {
  test_parent(t, n);
  test_level(t, n);
  test_children(t, n);
  test_node_location(t, n, l);
  test_pos_in_parent(t, n, l);
  test_node_neighbor(t, n, n.face_neighbors,
                     face_neighbors<Tree::dimension()>{}, l);
  test_node_neighbor(t, n, n.edge_neighbors,
                     edge_neighbors<Tree::dimension()>{}, l);
  test_node_neighbor(t, n, n.corner_neighbors,
                     corner_neighbors<Tree::dimension()>{}, l);
  test_node_neighbors(t, n, n.all_neighbors, l);
  test_normalized_coordinates(t, n, l);
}

template <typename Tree, typename ReferenceTree,
          typename Location = location::default_location<Tree::dimension()>>
void check_tree(Tree const& t, ReferenceTree const& tref,
                Location l = Location{}) {
  for (auto&& n : tref.nodes) { check_node(t, n, l); }
}

template <int nd>
auto uniformly_refined_tree(uint_t level, uint_t level_capacity) -> tree<nd> {
  auto node_capacity = no_nodes_until_uniform_level(nd, level_capacity);

  tree<nd> t(node_capacity);
  RANGES_FOR(auto&& n, t.nodes() | t.leaf()) {
    if (node_level(t, n) < level) { t.refine(n); }
  }

  CHECK(!t.empty());
  CHECK(size(t) == no_nodes_until_uniform_level(nd, level));

  return t;
}

}  // namespace test
