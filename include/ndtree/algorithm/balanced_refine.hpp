#pragma once
/// \file balanced_refine.hpp
#include <ndtree/algorithm/node_level.hpp>
#include <ndtree/algorithm/node_neighbors.hpp>
#include <ndtree/concepts.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
//

struct balanced_refine_fn {
  struct projection_fn {
    void operator()(node_idx) const noexcept {}
  };

  /// Refines node \p n of tree \p tree while maintaining the tree balanced
  ///
  /// \param tree [in] The tree on which the algorithm operates
  /// \param n [in] The node to refine within the tree
  /// \param p [in] A projection from the parent to its newly refined children
  ///               (useful for projecting data from the parent to its children)
  template <typename Tree, typename Projection = projection_fn,
            CONCEPT_REQUIRES_(Function<Projection, node_idx>{})>
  void operator()(Tree& tree, node_idx n, Projection&& p = Projection{}) const
   noexcept {
    if (!tree.is_leaf(n)) { return; }
    auto l = node_level(tree, n);
    for (auto&& neighbor : node_neighbors(tree, n)) {
      auto neighbor_level = node_level(tree, neighbor);
      if (neighbor_level == l - 1 and tree.is_leaf(neighbor)) {
        (*this)(tree, neighbor, p);
      }
    }
    tree.refine(n);
    p(n);
  }
};

namespace {
auto&& balanced_refine = static_const<balanced_refine_fn>::value;
}

}  // namespace v1

}  // namespace ndtree
