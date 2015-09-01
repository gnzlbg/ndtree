#pragma once
/// \file math.hpp Math utilities
#include <type_traits>
#include <ndtree/utility/ranges.hpp>

namespace ndtree {
inline namespace v1 {
/// Mathematical utilities
namespace math {

/// Computes b^e for (b,e) integers
///
/// TODO: assert on overflow
template <class Int, CONCEPT_REQUIRES_(Integral<Int>{})>
constexpr Int ipow(const Int b, const Int e) {
  return e == 0l ? 1l : b * ipow(b, e - static_cast<Int>(1));
}

/// Constexpr version of cmath floor
///
/// TODO: there has to be a better way
template <class Float, CONCEPT_REQUIRES_(std::is_floating_point<Float>{})>
constexpr Float floor(Float x) {
  return x >= 0.0 ? int(x) : int(x) - 1;
}

/// Computes the factorial of n
///
/// \param n [in] number whose factorial will be computed
///
/// TODO: assert on overflow
template <class Int, CONCEPT_REQUIRES_(Integral<Int>{})>
constexpr Int factorial(const Int n) noexcept {
  return (n == Int{0}) ? Int{1} : n * factorial(n - Int{1});
}

/// Computes the binomial coefficient (n m)
///
/// TODO: assert n - m >= 0 for unsigned types
template <class Int, CONCEPT_REQUIRES_(Integral<Int>{})>
constexpr Int binomial_coefficient(const Int n, const Int m) noexcept {
  return factorial(n) / (factorial(m) * factorial(n - m));
}

}  // namespace math
}  // namespace v1
}  // namespace ndtree
