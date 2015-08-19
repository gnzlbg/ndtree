#pragma once
/// \file find_node_or_parent.hpp
#include <ndtree/location.hpp>
#include <ndtree/types.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
//

/// Find the index of a node at a given location
///
/// Returns the closest parent of the node if the node at exact location was not
/// found
struct find_node_or_parent_fn {
  template <typename Tree>
  auto operator()(Tree const& t, location<Tree::dimension()> loc) const noexcept
   -> node_idx {
    node_idx n = 0_n;
    for (auto&& p : loc()) {
      auto m = t.child(n, typename Tree::child_pos{p});
      if (!m) { break; }
      n = m;
    }
    return n;
  }
};

namespace {
constexpr auto&& find_node_or_parent
 = static_const<find_node_or_parent_fn>::value;
}

}  // namespace v1
}  // namespace ndtree
