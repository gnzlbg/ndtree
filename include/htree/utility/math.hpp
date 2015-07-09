#pragma once
/// \file math.hpp Math utilities
#include <htree/utility/ranges.hpp>

namespace htree {
/// Mathematical utilities
namespace math {

/// Computes b^e for (b,e) integers
///
/// Note: overflows for e >= (64-1) / ilog(b,2);
constexpr long ipow(const long b, const long e) {
  return e == 0l ? 1l : b * ipow(b, e - 1l);
}

/// Constexpr version of cmath floor
///
/// TODO: improve this
template <class Float, CONCEPT_REQUIRES_(std::is_floating_point<Float>{})>
constexpr Float floor(Float x) {
  return x >= 0.0 ? int(x) : int(x) - 1;
}

}  // namespace math
}  // namespace htree
