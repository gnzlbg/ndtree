#pragma once
/// \file shift_location.hpp
#include <array>
#include <ndtree/location.hpp>
#include <ndtree/types.hpp>
#include <ndtree/relations/tree.hpp>
#include <ndtree/utility/math.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
//

/// Shift node location
struct shift_location_fn {
  template <int nd>
  auto operator()(location<nd> loc, std::array<num_t, nd> offset,
                  uint_t level = location<nd>::max_level()) const noexcept
   -> location<nd> {
    const num_t scale = math::ipow(2, loc.level);
    for (auto&& d : dimensions(nd)) { offset[d] += loc[d] / scale; }
    return location<nd>{offset, level};
  }

  template <int nd>
  auto operator()(location<nd> loc, std::array<int_t, nd> offset) const noexcept
   -> optional_location<nd> {
    for (auto&& d : dimensions(nd)) {
      const optional_location<nd> new_loc = shift(loc, d, offset[d]);
      if (!new_loc) { return new_loc; }
      loc = (*new_loc);
    }
    return loc;
  }
};

namespace {
constexpr auto&& shift_location = static_const<shift_location_fn>::value;
}

}  // namespace v1
}  // namespace ndtree
