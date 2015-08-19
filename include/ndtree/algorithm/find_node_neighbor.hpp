#pragma once
/// \file find_node_neighbor.hpp
#include <ndtree/types.hpp>
#include <ndtree/location.hpp>
#include <ndtree/relations.hpp>
#include <ndtree/algorithm/shift_location.hpp>
#include <ndtree/algorithm/find_node.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
/// Find the index of a node at a given location
struct find_node_neighbor_fn {
  template <typename Tree, typename neighbor_idx, int nd = Tree::dimension(),
            typename manifold = get_tag_t<neighbor_idx>>
  auto operator()(Tree const& t, location<nd> loc, neighbor_idx n) const
   noexcept -> node_idx {
    return find_node(t, shift_location(loc, manifold {}[n]));
  }
};

namespace {
constexpr auto&& find_node_neighbor
 = static_const<find_node_neighbor_fn>::value;
}

}  // namespace v1
}  // namespace ndtree
