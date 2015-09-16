#pragma once
/// \file node_neighbor.hpp
#include <ndtree/algorithm/shift_location.hpp>
#include <ndtree/algorithm/node_at.hpp>
#include <ndtree/concepts.hpp>
#include <ndtree/types.hpp>
#include <ndtree/relations/neighbor.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
/// Find neighbor \p n of node at location \p loc
///
/// Note: the manifold is associated to the neighbor index type
/// (todo: strongly type this)
///
struct node_neighbor_fn {
  template <typename Tree, typename Loc, typename NeighborIdx,
            typename Manifold = get_tag_t<NeighborIdx>,
            CONCEPT_REQUIRES_(Location<Loc>{})>
  auto operator()(Tree const& t, Loc&& loc, NeighborIdx n) const noexcept
   -> node_idx {
    static_assert(Tree::dimension() == ranges::uncvref_t<Loc>::dimension(), "");
    static_assert(Tree::dimension() == Manifold::dimension(), "");
    return node_at(t, shift_location(loc, Manifold {}[n]));
  }
};

namespace {
constexpr auto&& node_neighbor = static_const<node_neighbor_fn>::value;
}  // namespace

}  // namespace v1
}  // namespace ndtree
