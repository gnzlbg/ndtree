#pragma once
/// \file node_neighbors.hpp
#include <ndtree/algorithm/node_location.hpp>
#include <ndtree/algorithm/node_or_parent_at.hpp>
#include <ndtree/algorithm/shift_location.hpp>
#include <ndtree/concepts.hpp>
#include <ndtree/location/default.hpp>
#include <ndtree/relations/neighbor.hpp>
#include <ndtree/types.hpp>
#include <ndtree/utility/static_const.hpp>
#include <ndtree/utility/stack_vector.hpp>

namespace ndtree {
inline namespace v1 {
//

struct node_neighbors_fn {
  /// Finds neighbors of node at location \p loc across the Manifold
  /// (appends them to a push_back-able container)
  template <typename Manifold, typename Tree, typename Loc,
            typename PushBackableContainer, CONCEPT_REQUIRES_(Location<Loc>{})>
  auto operator()(Manifold positions, Tree const& t, Loc&& loc,
                  PushBackableContainer& s) const noexcept -> void {
    static_assert(Tree::dimension() == ranges::uncvref_t<Loc>::dimension(), "");
    // For all same level neighbor positions
    for (auto&& sl_pos : positions()) {
      auto neighbor
       = node_or_parent_at(t, shift_location(loc, positions[sl_pos]));
      if (!neighbor.idx) { continue; }
      NDTREE_ASSERT((neighbor.level == loc.level())
                     || (neighbor.level == (loc.level() - 1)),
                    "found neighbor must either be at the same level or at the "
                    "parent level");

      // if the neighbor found is a leaf node we are done
      // note: it is either at the same or parent level of the node
      // (doesn't matter which case it is, it is the correct neighbor)
      if (t.is_leaf(neighbor.idx)) {
        s.push_back(neighbor.idx);
      } else {
        // if it has children we add the children sharing a face with the node
        for (auto&& cp : Manifold{}.children_sharing_face(sl_pos)) {
          s.push_back(t.child(neighbor.idx, cp));
        }
      }
    }
  }

  /// Finds neighbors of node at location \p loc across the Manifold
  ///
  /// \returns stack allocated vector containing the neighbors
  template <typename Manifold, typename Tree, typename Loc,
            int max_no_neighbors = Manifold::no_child_level_neighbors(),
            CONCEPT_REQUIRES_(Location<Loc>{})>
  auto operator()(Manifold, Tree const& t, Loc&& loc) const noexcept
   -> stack_vector<node_idx, max_no_neighbors> {
    static_assert(Tree::dimension() == ranges::uncvref_t<Loc>::dimension(), "");
    stack_vector<node_idx, max_no_neighbors> neighbors;
    (*this)(Manifold{}, t, loc, neighbors);
    return neighbors;
  }

  /// Finds set of unique neighbors of node at location \p loc across all
  /// manifolds
  ///
  /// \param t [in] tree.
  /// \param loc [in] location (location of the node).
  /// \returns stack allocated vector containing the unique set of neighbors
  ///
  template <typename Tree, typename Loc, int nd = Tree::dimension(),
            CONCEPT_REQUIRES_(Location<Loc>{})>
  auto operator()(Tree const& t, Loc&& loc) const noexcept
   -> stack_vector<node_idx, max_no_neighbors(nd)> {
    stack_vector<node_idx, max_no_neighbors(nd)> neighbors;

    // For each surface manifold append the neighbors
    using manifold_rng = meta::as_list<meta::integer_range<int, 1, nd + 1>>;
    meta::for_each(manifold_rng{}, [&](auto m_) {
      using manifold = manifold_neighbors<nd, decltype(m_){}>;
      (*this)(manifold{}, t, loc, neighbors);
    });

    // sort them and remove dupplicates
    ranges::sort(neighbors);
    neighbors.erase(ranges::unique(neighbors), end(neighbors));
    return neighbors;
  }

  /// Finds set of unique neighbors of node \p n across all manifolds
  ///
  /// \param t [in] tree.
  /// \param n [in] node index.
  /// \returns stack allocated vector containing the unique set of neighbors
  ///
  template <typename Tree,
            typename Loc = location::default_location<Tree::dimension()>,
            CONCEPT_REQUIRES_(Location<Loc>{})>
  auto operator()(Tree const& t, node_idx n, Loc l = Loc{}) const noexcept {
    return (*this)(t, node_location(t, n, l));
  }
};

namespace {
constexpr auto&& node_neighbors = static_const<node_neighbors_fn>::value;
}  // namespace

}  // namespace v1
}  // namespace ndtree
