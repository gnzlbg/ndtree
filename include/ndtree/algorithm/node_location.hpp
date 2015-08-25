#pragma once
/// \file node_location.hpp
#include <ndtree/location.hpp>
#include <ndtree/types.hpp>
#include <ndtree/utility/assert.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
//

struct node_location_fn {
  /// Location code of the node with index \p n within the tree \p t
  template <typename Tree>
  auto operator()(Tree const& t, node_idx n) const noexcept
   -> location<Tree::dimension()> {
    NDTREE_ASSERT(n, "cannot compute the location of an invalid node");
    location<Tree::dimension()> loc;

    while (!t.is_root(n)) {
      loc.push(Tree::position_in_parent(n));
      n = t.parent(n);
    }

    loc.reverse();
    return loc;
  }
};

namespace {
constexpr auto&& node_location = static_const<node_location_fn>::value;
}

}  // namespace v1
}  // namespace ndtree
