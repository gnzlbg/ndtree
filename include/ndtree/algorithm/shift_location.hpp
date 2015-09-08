#pragma once
/// \file shift_location.hpp
#include <array>
#include <ndtree/concepts.hpp>
#include <ndtree/types.hpp>
#include <ndtree/relations/tree.hpp>
#include <ndtree/utility/math.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
//

/// Shift node location
struct shift_location_fn {
  /// Shifts the location \p loc by normalized \p offset at \p level
  ///
  /// TODO: if offset is out of bounds -> undefined behavior (right now
  /// an assertion triggers in location's constructor)
  template <typename Loc, int nd = Loc::dimension(),
            CONCEPT_REQUIRES_(Location<Loc>{})>
  auto operator()(Loc loc, std::array<num_t, nd> offset,
                  uint_t level = Loc::max_level()) const noexcept -> Loc {
    const num_t scale = math::ipow(2, loc.level);
    for (auto&& d : dimensions(nd)) { offset[d] += loc[d] / scale; }
    return Loc{offset, level};
  }

  /// Shifts the location \p loc by \p offset
  ///
  /// If the resulting location is out-of-bounds the optional_location won't
  /// contain a valid value.
  template <typename Loc, uint_t nd = Loc::dimension(),
            CONCEPT_REQUIRES_(Location<Loc>{})>
  auto operator()(Loc loc, std::array<int_t, nd> offset) const noexcept
   -> compact_optional<Loc> {
    return shift(loc, offset);
  }
};

namespace {
constexpr auto&& shift_location = static_const<shift_location_fn>::value;
}

}  // namespace v1
}  // namespace ndtree
