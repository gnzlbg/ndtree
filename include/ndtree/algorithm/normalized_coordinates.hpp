#pragma once
/// \file normalized_coordinates.hpp
#include <array>
#include <ndtree/concepts.hpp>
#include <ndtree/types.hpp>
#include <ndtree/relations/tree.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
//

struct normalized_coordinates_fn {
  /// Returns the position of the location \p loc in normalized coordinates
  ///
  /// \note normalized coordinates means normalized by the length of the root
  /// node, that is, the coordinates are in range (0., 1.).
  template <typename Loc, int nd = Loc::dimension(),
            CONCEPT_REQUIRES_(Location<Loc>{})>
  auto operator()(Loc loc) const noexcept -> std::array<num_t, nd> {
    std::array<num_t, nd> result;
    for (auto&& i : result) { i = 0.5; }

    uint_t l = 0;
    for (auto p : loc()) {
      const auto length = node_length_at_level(l) * num_t{0.25};
      for (auto d : dimensions(nd)) {
        result[d] += relative_child_position(p, d) * length;
      }
      ++l;
    }
    return result;
  }

  /// Returns the position of node \p n in normalized coordinates within the
  /// tree \p t
  ///
  /// \note normalized coordinates means normalized by the length of the root
  /// node, that is, the coordinates are in range (0., 1.).
  template <typename Tree, typename Loc, CONCEPT_REQUIRES_(Location<Loc>{})>
  auto operator()(Tree const& t, node_idx n, Loc l) const noexcept {
    return (*this)(node_location(t, n, l));
  }
};

namespace {
constexpr auto&& normalized_coordinates
 = static_const<normalized_coordinates_fn>::value;
}  // namespace

}  // namespace v1
}  // namespace ndtree
