#pragma once
/// \file node_neighbor.hpp
#include <ndtree/types.hpp>
#include <ndtree/location.hpp>
#include <ndtree/relations/neighbor.hpp>
#include <ndtree/algorithm/shift_location.hpp>
#include <ndtree/algorithm/node_at.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
/// Find neighbor \p n of node at location \p loc
///
/// Note: the manifold is associated to the neighbor index type
/// (todo: strongly type this)
///
struct node_neighbor_fn {
  template <typename Tree, typename neighbor_idx, int nd = Tree::dimension(),
            typename manifold = get_tag_t<neighbor_idx>>
  auto operator()(Tree const& t, location<nd> loc, neighbor_idx n) const
   noexcept -> node_idx {
    return node_at(t, shift_location(loc, manifold {}[n]));
  }
};

namespace {
constexpr auto&& node_neighbor = static_const<node_neighbor_fn>::value;
}

}  // namespace v1
}  // namespace ndtree
