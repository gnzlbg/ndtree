#pragma once
/// \file tree.hpp
///
/// TODO:
/// - replace int static casts with something better
///
#include <htree/utility/assert.hpp>
#include <htree/utility/fmt.hpp>
#include <htree/utility/math.hpp>
#include <htree/utility/ranges.hpp>
#include <htree/utility/tagged_sentinel_optional.hpp>
#include <htree/utility/tagged_ranged.hpp>

namespace htree {

struct node_idx_tag {};
using node_idx = tagged_sentinel_optional<int, node_idx_tag>;

constexpr node_idx operator"" _n(unsigned long long int i) {
  return node_idx{static_cast<int>(i)};
}

struct siblings_tag {};
using siblings_idx = tagged_sentinel_optional<int, siblings_tag>;

constexpr siblings_idx operator"" _sg(unsigned long long int i) {
  return siblings_idx{static_cast<int>(i)};
}

template <int nd> struct tree {
  /// \name Data
  ///
  /// All member variables of the tree.
  ///
  ///@{
  /// Parent of a children group
  std::vector<node_idx> parents_;
  /// First child node of a given node
  std::vector<node_idx> first_children_;
  std::size_t size_ = 0;
  siblings_idx first_free_sibling_group_{0};
  ///@}  // Data

  /// \name Spatial constants
  ///@{

  /// Number of spatial dimensions
  static constexpr int dimension() noexcept { return nd; }

  /// Range of spatial dimensions
  static constexpr auto dimensions() noexcept { return view::iota(0, nd); }
  HTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(dimensions());

  /// Number of children per node
  static constexpr int no_children() noexcept { return math::ipow(2, nd); }

  ///@}  // Spatial constants

  /// \name Graph edges (parent/children)
  ///
  ///@{

 private:
  /// Sibling group of node \p n
  static constexpr siblings_idx sibling_group(node_idx n) noexcept {
    HTREE_ASSERT(n, "cannot compute sibling group of invalid node");
    return siblings_idx{
     static_cast<int>(math::floor((*n + no_children() - 1.f) / no_children()))};
  }

  /// Parent node of sibling group \p s
  node_idx parent(siblings_idx s) const noexcept {
    HTREE_ASSERT(s, "cannot obtain parent of invalid sibling group");
    HTREE_ASSERT(*s >= 0 and static_cast<std::size_t>(*s) < parents_.size(),
                 "sg {} is out-of-bounds for parents [{}, {})", *s, 0,
                 parents_.size());
    return parents_[*s];
  }

  /// Mutate the parent node of the sibling group \p to \p value
  ///
  /// \warning not thread-safe
  void set_parent(siblings_idx s, node_idx value) noexcept {
    HTREE_ASSERT(s, "cannot set parent of invalid sibling group");
    HTREE_ASSERT(*s >= 0 and static_cast<std::size_t>(*s) < parents_.size(),
                 "sg {} is out-of-bounds for parents [{}, {})", *s, 0,
                 parents_.size());
    parents_[*s] = value;
  }

 public:
  /// Parent node of node \p
  node_idx parent(node_idx n) const noexcept {
    return parent(sibling_group(n));
  }

  struct child_pos_tag {};
  using child_pos = tagged_ranged<int, 0, no_children(), child_pos_tag>;

 private:
  /// First children of node \p n
  node_idx first_child(node_idx n) const noexcept {
    HTREE_ASSERT(n, "cannot obtain first child of invalid node");
    HTREE_ASSERT(*n >= 0
                  and static_cast<std::size_t>(*n) < first_children_.size(),
                 "node {} is out-of-bounds for first_child [{}, {})", *n, 0,
                 first_children_.size());
    return first_children_[*n];
  }

  /// Mutate the value of the first child of node \p to value
  ///
  /// \warning not thread-safe
  void set_first_child(node_idx n, node_idx value) noexcept {
    HTREE_ASSERT(n, "cannot set first child of invalid node");
    HTREE_ASSERT(*n >= 0
                  and static_cast<std::size_t>(*n) < first_children_.size(),
                 "node {} is out-of-bounds for first_child [{}, {})", *n, 0,
                 first_children_.size());
    first_children_[*n] = value;
  }

  /// Sibling group of children of node \p n
  siblings_idx children_group(node_idx n) const noexcept {
    auto c = first_child(n);
    return c ? sibling_group(c) : siblings_idx{};
  }

  /// First node in sibling group \p s
  static constexpr node_idx first_node(siblings_idx s) noexcept {
    return (*s == 0) ? 0_n : node_idx{1 + no_children() * (*s - 1)};
  }

 public:
  /// Range of child positions
  static constexpr auto child_positions() noexcept { return child_pos::rng(); }

  /// Child node at position \p p of node \p n
  node_idx child(node_idx n, child_pos p) const noexcept {
    const auto fc = first_child(n);
    return fc ? node_idx{*fc + *p} : fc;
  }

  /// Range of children nodes of node \p n
  auto children(node_idx n) const noexcept {
    const auto fc = first_child(n);
    return fc ? node_idx::rng(*fc, *fc + no_children()) : node_idx::rng(0, 0);
  }

 private:
  /// First node of the tree
  static constexpr node_idx first_node() noexcept { return 0_n; }
  /// Last node of the tree
  node_idx last_node() const noexcept {
    return node_idx{static_cast<int>(capacity())};
  }

  /// First sibling group of the tree
  static constexpr siblings_idx first_sg() noexcept { return 0_sg; }
  /// Last sibling group of the tree
  constexpr siblings_idx last_sg() const noexcept {
    return sibling_group(last_node());
  }

  /// Nodes in sibling group \p s
  static constexpr auto nodes(siblings_idx s) noexcept {
    const auto fn = first_node(s);
    return !is_root(s) ? node_idx::rng(*fn, *fn + no_children())
                       : node_idx::rng(0_n, 1_n);
  }

 public:
  /// All nodes in use within the tree (fast and safe)
  ///
  /// \pre the tree must be compact
  auto operator()() const noexcept {
    HTREE_ASSERT(is_compact(), "fast node range requires compact tree");
    return node_idx::rng(first_node(), last_node());
  }

  /// All nodes in use within the tree (slow and safe)
  ///
  /// \note The tree does not need to be compact
  auto nodes() const noexcept {
    // Explanation:
    // create range of all sibling groups (including free ones)
    // | filter out free sibling groups (only sibling groups in use remain)
    // | transform each sibling group into a range of its nodes:
    //   this produces a range of ranges of nodes
    // | flatten out the range
    return sibling_groups()
           | view::transform([](siblings_idx s) { return nodes(s); })
           | view::join;
  }

  /// Selects leaf nodes
  auto leaf() const noexcept {
    return view::filter([&](node_idx i) { return is_leaf(i); });
  }

  /// Selects nodes with children
  auto with_children() const noexcept {
    return view::remove_if([&](node_idx i) { return is_leaf(i); });
  }

 private:
  auto sibling_groups() const noexcept {
    return siblings_idx::rng(first_sg(), last_sg())
           | view::filter([&](siblings_idx s) { return !is_free(s); });
  }

 private:
  /// Is \p s the sibling group of the root node?
  static constexpr bool is_root(siblings_idx s) noexcept { return *s == 0; }

 public:
  /// Is \p n the root node?
  static constexpr bool is_root(node_idx n) noexcept { return *n == 0; }

 private:
  /// A sibling group is free if it doesn't have a parent
  /// (unless it is the root node, which is never empty)
  bool is_free(siblings_idx s) const noexcept {
    if (HTREE_UNLIKELY(is_root(s))) { return false; }
    return !parent(s);
  }

  /// Is a node part of a free sibling group?
  bool is_free(node_idx n) const noexcept { return is_free(sibling_group(n)); }

 public:
  /// Is node \p n a leaf node? (That is, does it have zero children?)
  bool is_leaf(node_idx n) const noexcept { return !first_child(n); }

  /// Number of childrens of the node \p n
  int no_children(node_idx n) const noexcept {
    return is_leaf(n) ? 0 : no_children();
  }

  template <class F> void traverse_parents(node_idx n, F&& f) const noexcept {
    while ((n = parent(n))) { f(n); };
  }

  /// Level of node \p n within the tree
  int level(node_idx n) const noexcept {
    HTREE_ASSERT(!is_free(n), "node {} is free and doesn't have a level", *n);
    int l = 0;
    traverse_parents(n, [&](node_idx) { ++l; });
    return l;
  }

  /// Does the tree have a compact representation?
  ///
  /// That is, no free sibling groups before the last sibling group in use.
  ///
  bool is_compact() const noexcept {
    return first_free_sibling_group_
           == sibling_group(node_idx{static_cast<int>(size())});
  }

  ///@}  // Graph edges (parent/children)

  /// \name Memory management
  ///@{

  /// Node capacity
  std::size_t capacity() const noexcept {
    return 1 + (ranges::size(parents_) - 1) * no_children();
  }

  /// Node size
  std::size_t size() const noexcept { return size_; }

  /// Is the tree empty?
  bool empty() const noexcept { return size_ == 0; }

  /// Refine node \p p and returns children group idx
  ///
  /// If refinement failed returns nothing.
  ///
  /// \pre !is_free(p) && is_leaf(p)
  /// \post !is_free(p) && !is_leaf(p)
  siblings_idx refine(node_idx p) noexcept {
    HTREE_ASSERT(!is_free(p), "node {}: is free and cannot be refined", *p);
    HTREE_ASSERT(is_leaf(p), "node {}: is not a leaf and cannot be refined",
                 *p);
    if (size() == capacity()) { return siblings_idx{}; }

    const auto s = first_free_sibling_group_;
    HTREE_ASSERT(is_free(s), "node {}: first free sg {} is not free", *p, *s);

    size_ += no_children();

    (*first_free_sibling_group_)++;
    auto remaining_sgs
     = siblings_idx::rng(first_free_sibling_group_, last_sg());
    HTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(remaining_sgs);

    auto next_unused_sg_it
     = find_if(remaining_sgs, [&](siblings_idx i) { return is_free(i); });
    if (next_unused_sg_it != end(remaining_sgs)) {
      first_free_sibling_group_ = *next_unused_sg_it;
    }

    set_parent(s, p);
    set_first_child(p, first_node(s));

    HTREE_ASSERT(!is_free(s), "node {}: refine produced a free sg {}", *p, *s);
    HTREE_ASSERT(all_of(children(p), [&](node_idx i) { return is_leaf(i); }),
                 "node {}: refine produced non leaf children", *p);
    HTREE_ASSERT(!is_free(p), "node {}: refine freed the node", *p);
    HTREE_ASSERT(!is_leaf(p), "node {}: refine produced a leaf parent node",
                 *p);
    return s;
  }

  /// Coarsen node \p p
  ///
  /// \pre !is_free(p) && !is_leaf(p) && is_leaf(children group of p)
  /// \post !is_free(p) && is_leaf(p) && is_free(children group of p)
  void coarsen(node_idx p) noexcept {
    HTREE_ASSERT(!is_free(p), "node {}: is free, cannot coarsen", *p);
    HTREE_ASSERT(!is_leaf(p), "node {}: is leaf, cannot coarsen", *p);

    const auto cg = children_group(p);
    HTREE_ASSERT(!is_free(cg), "node {}: its child group {} is free", *p, *cg);

    size_ -= no_children();

    if (*cg < *first_free_sibling_group_) { first_free_sibling_group_ = cg; }

    set_parent(cg, node_idx{});
    set_first_child(p, node_idx{});

    HTREE_ASSERT(is_free(cg), "node {}: after coarsen child group {} not free",
                 *p, *cg);
    HTREE_ASSERT(is_leaf(p), "node {}: after coarsen not leaf", *p);
    HTREE_ASSERT(!is_free(p), "node {}: after coarsen is free", *p);
  }

 private:
  /// Initializes the tree with a root node
  ///
  /// \pre empty()
  /// \post size() == 1
  void initialize_root_node() {
    HTREE_ASSERT(empty(), "cannot initialize root node on non-empty tree");
    HTREE_ASSERT(!parent(0_sg), "first sibling group has a parent");
    HTREE_ASSERT(is_leaf(0_n), "root node already has children");
    size_++;
    (*first_free_sibling_group_)++;
    HTREE_ASSERT(size() == 1, "after root node init size is {} and not 1",
                 size());
  }

  /// Is the tree reseted?
  bool is_reseted() {
    return size_ == 0 and first_free_sibling_group_ == 0_sg
           and all_of(parents_, [](node_idx i) { return !i; })
           and all_of(first_children_, [](node_idx i) { return !i; });
  }

  /// Swaps the memory location of the sibling groups \p a and \p b
  ///
  /// \pre no sibling group can be swapped with the root's node sibling group
  /// \pre no sibling group can be swapped with itself
  void swap(siblings_idx a, siblings_idx b) noexcept {
    HTREE_ASSERT(a != 0_sg and b != 0_sg, "root node is not swappable");
    HTREE_ASSERT(a || b, "at least one of both sg must be valid");
    HTREE_ASSERT(a != b, "self-swap not allowed for sg {}", a ? *a : -1);

    // 0) Break early: both not in use -> nothing to do
    if (is_free(a) and is_free(b)) { return; }

    /// 1) swap siblings -> children edges, and children -> sibling edges:
    auto update_cg_parent = [&](node_idx s) {
      const auto child_cg = children_group(s);
      if (child_cg) { set_parent(child_cg, s); }
    };
    for (auto n : view::zip(nodes(a), nodes(b))) {
      auto l = get<0>(n), r = get<1>(n);
      ranges::swap(first_children_[*l], first_children_[*r]);
      update_cg_parent(l);
      update_cg_parent(r);
    };

    /// 2) swap parent -> sibling edges, and sibling -> parent edges:
    auto update_parent_sibling_e = [&](node_idx parent, siblings_idx s) {
      if (parent) { set_first_child(parent, first_node(s)); }
      set_parent(s, parent);
    };
    {
      auto p_a = parent(a);
      auto p_b = parent(b);
      update_parent_sibling_e(p_a, b);
      update_parent_sibling_e(p_b, a);
    }

    /// 3) update the first free sibling group flag:
    ///    - if one of the nodes is inactive:
    auto update_first_free_sg = [&](siblings_idx i, siblings_idx j) {
      if (is_free(i)) {
        first_free_sibling_group_
         = (j == first_free_sibling_group_) ? i : first_free_sibling_group_;
      }
    };
    update_first_free_sg(a, b);
    update_first_free_sg(b, a);
  }

  ///@}  // Memory management

 public:
  tree(std::size_t capacity_)
   : parents_(*sibling_group(node_idx{static_cast<int>(capacity_ - 1)}) + 1)
   , first_children_(capacity()) {
    HTREE_ASSERT(capacity_ > 0, "cannot construct tree with zero capacity ({})",
                 capacity_);
    HTREE_ASSERT(is_reseted(), "tree is not reseted");
    initialize_root_node();
  }

  /// \name Spatial algorithms
  ///@{

 private:
  /// Sorts the tree in depth-first order, with the siblings of each group
  /// sorted in Morton Z-Curve order
  ///
  /// Runtime complexity: O(N), where N is the number of nodes in the tree.
  /// Space complexity: O(log(N)) stack frames.
  ///
  /// \param s [in] Start sibling group. All nodes below it will be sorted
  ///
  /// \pre \p s is at its correct position
  ///
  /// The algorithm is recursive. It assumes that the input group \p s is at its
  /// correct position in memory. It then traverses the children groups of its
  /// siblings in deapth-first order. If the children group of a sibling is not
  /// at its correct position, it is swapped with the group at the correct
  /// position.
  ///
  /// \post is_compact() && is_sorted()
  siblings_idx sort(siblings_idx s) noexcept {
    siblings_idx should = s;
    for (auto n : nodes(s) | with_children()) {
      ++should;

      siblings_idx c_sg = children_group(n);

      if (c_sg != should) {
        swap(c_sg, should);
        should = sort(should);
      } else {
        should = sort(c_sg);
      }
    }

    return should;
  }

 public:
  /// Sorts the tree in depth-first order, with the siblings of each group
  /// sorted in Morton Z-Curve order
  ///
  /// Runtime complexity: O(N), where N is the number of nodes in the tree.
  /// Space complexity: O(log(N)) stack frames.
  ///
  /// \post is_compact() && is_sorted()
  void sort() noexcept {
    sort(0_sg);
    first_free_sibling_group_ = sibling_group(node_idx{(int)size()});
    HTREE_ASSERT(is_compact(), "the tree must be compact after sorting");
  }
  ///@}
};

}  // namespace htree
