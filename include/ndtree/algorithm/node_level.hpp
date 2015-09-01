#pragma once
/// \file node_level.hpp
#include <ndtree/types.hpp>
#include <ndtree/location.hpp>
#include <ndtree/algorithm/root_traversal.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
//

struct node_level_fn {
  /// Level of the node \p n within the tree \p tree
  ///
  /// \param tree [in] Tree.
  /// \param n [in] Node index.
  ///
  /// Time complexity: O(log(N))
  /// Space complexity: O(1)
  template <typename Tree>
  auto operator()(Tree const& tree, node_idx n) const noexcept -> uint_t {
    uint_t l = 0;
    root_traversal(tree, tree.parent(n), [&](node_idx) {
      ++l;
      return true;
    });
    return l;
  }
  /// Level of node at location \p loc
  ///
  /// \param loc [in] Node location.
  ///
  /// Time complexity: O(1)
  /// Space complexity: O(1)
  template <int nd>
  auto operator()(location<nd> const& loc) const noexcept -> uint_t {
    return loc.level;
  }
};

namespace {
constexpr auto&& node_level = static_const<node_level_fn>::value;
}

}  // namespace v1
}  // namespace ndtree
