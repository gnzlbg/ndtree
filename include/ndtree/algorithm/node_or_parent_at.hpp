#pragma once
/// \file node_or_parent_at.hpp
#include <ndtree/types.hpp>
#include <ndtree/concepts.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
//

struct node_or_parent_at_fn {
  struct node {
    node_idx idx{};
    uint_t level = 0_u;
  };

  /// Index of smallest node containing \p loc with level <= loc.level
  ///
  /// \param t [in] n-dimensional tree.
  /// \param loc [in] location code.
  ///
  /// \returns node(index, level) of the smallest node containing \p loc with
  /// level <= loc.level
  ///
  template <typename Tree, typename Loc, CONCEPT_REQUIRES_(Location<Loc>{})>
  auto operator()(Tree const& t, Loc&& loc) const noexcept -> node {
    static_assert(Tree::dimension() == ranges::uncvref_t<Loc>::dimension(), "");
    node result{0_n, 0_u};
    for (auto&& p : loc()) {
      auto m = t.child(result.idx, typename Tree::child_pos{p});
      if (!m) { break; }
      result.idx = m;
      ++result.level;
    }
    return result;
  }
  template <typename Tree, typename Loc, CONCEPT_REQUIRES_(Location<Loc>{})>
  auto operator()(Tree& t, compact_optional<Loc> loc) const noexcept -> node {
    return loc ? (*this)(t, *loc) : node{};
  }
};

namespace {
constexpr auto&& node_or_parent_at = static_const<node_or_parent_at_fn>::value;
}  // namespace

}  // namespace v1
}  // namespace ndtree
