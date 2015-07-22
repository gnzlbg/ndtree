#pragma once
/// \file find_node.hpp
#include <ndtree/location.hpp>
#include <ndtree/types.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
//

/// Find the index of a node at a given location
struct find_node_fn {
  template <typename Tree>
  auto operator()(Tree const& t, location<Tree::dimension()> loc) const
   noexcept -> node_idx {
    node_idx n = 0_n;
    for (auto&& p : loc()) {
      n = t.child(n, typename Tree::child_pos{p});
      if (!n) { break; }
    }
    return n;
  }
};

namespace {
constexpr auto&& find_node = static_const<find_node_fn>::value;
}

}  // namespace v1
}  // namespace ndtree
