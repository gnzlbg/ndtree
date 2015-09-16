#pragma once
/// \file node_at.hpp
#include <ndtree/concepts.hpp>
#include <ndtree/types.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
//

struct node_at_fn {
  /// Index of node at level loc.level containing the location \p loc
  ///
  /// \param t [in] n-dimensional tree.
  /// \param loc [in] location code.
  /// \param n [in] root node of the location code within the tree \p t
  ///
  /// \returns index of node at level loc.level() containing loc. If no node is
  /// found at the same level of \p loc or the location is invalid returns an
  /// invalid node
  ///
  template <typename Tree, typename Loc, CONCEPT_REQUIRES_(Location<Loc>{})>
  auto operator()(Tree const& t, Loc&& loc, node_idx n = 0_n) const noexcept
   -> node_idx {
    static_assert(Tree::dimension() == std::decay_t<Loc>::dimension(), "");
    for (auto&& p : loc()) {
      n = t.child(n, child_pos<Tree>{p});
      if (!n) { return node_idx{}; }
    }
    return n;
  }

  template <typename Tree, typename Loc, CONCEPT_REQUIRES_(Location<Loc>{})>
  auto operator()(Tree& t, compact_optional<Loc> loc, node_idx n = 0_n) const
   noexcept -> node_idx {
    return loc ? (*this)(t, *loc, n) : node_idx{};
  }
};

namespace {
constexpr auto&& node_at = static_const<node_at_fn>::value;
}  // namespace

}  // namespace v1
}  // namespace ndtree
