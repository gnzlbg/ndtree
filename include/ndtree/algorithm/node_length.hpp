#pragma once
/// \file node_length.hpp
#include <ndtree/location.hpp>
#include <ndtree/types.hpp>
#include <ndtree/relations/tree.hpp>
#include <ndtree/utility/static_const.hpp>
#include <ndtree/algorithm/node_location.hpp>

namespace ndtree {
inline namespace v1 {
//

/// Find the index of a node at a given location
struct node_length_fn {
  /// Normalized length of the node at location \p loc
  ///
  /// \param loc [in] Node location
  ///
  /// Time complexity: O(1)
  template <int nd> auto operator()(location<nd> loc) const noexcept -> num_t {
    return node_length_at_level(loc.level);
  }

  /// Normalized length of the node \p n within the tree \p t
  ///
  /// \param t [in] Tree
  /// \param n [in] Node index
  ///
  /// Time complexity: O(log(N))
  template <typename Tree>
  auto operator()(Tree const& t, node_idx n) const noexcept -> num_t {
    return (*this)(node_location(t, n));
  }
};

namespace {
constexpr auto&& node_length = static_const<node_length_fn>::value;
}

}  // namespace v1
}  // namespace ndtree
