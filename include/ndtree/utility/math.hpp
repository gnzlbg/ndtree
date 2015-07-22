#pragma once
/// \file math.hpp Math utilities
#include <type_traits>
#include <ndtree/utility/ranges.hpp>

namespace ndtree {
inline namespace v1 {
/// Mathematical utilities
namespace math {

/// Computes b^e for (b,e) integers
template <class Int, CONCEPT_REQUIRES_(std::is_integral<Int>{})>
constexpr Int ipow(const Int b, const Int e) {
  return e == 0l ? 1l : b * ipow(b, e - static_cast<Int>(1));
}

/// Constexpr version of cmath floor
///
/// TODO: improve this
template <class Float, CONCEPT_REQUIRES_(std::is_floating_point<Float>{})>
constexpr Float floor(Float x) {
  return x >= 0.0 ? int(x) : int(x) - 1;
}

}  // namespace math
}  // namespace v1
}  // namespace ndtree
