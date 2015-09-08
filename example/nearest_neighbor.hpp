#pragma once
/// \file nearest_neighbor.hpp Nearest neighbor container
#include <utility>
#include <vector>
#include <unordered_map>
#include <ndtree/tree.hpp>
#include <ndtree/algorithm.hpp>
#include <ndtree/location/fast.hpp>
#include <ndtree/utility/stack_vector.hpp>

namespace nearest_neighbor {

/// import ndtree namespace
using namespace ndtree;

template <uint_t nd> using vec = std::array<num_t, nd>;  // num_t == double

/// Computes the distance between two points
template <uint_t nd>  //
num_t distance(vec<nd> a, vec<nd> b) noexcept {
  num_t d = 0;
  RANGES_FOR(auto&& p, ranges::view::zip(a, b)) {
    d += std::pow(std::get<0>(p) - std::get<1>(p), 2);
  }
  return std::sqrt(d);
}

/// Brute forces the nearest neighbor of \p in the vector of points \points
template <uint_t nd>
vec<nd> nn_brute_force(std::vector<vec<nd>> const& points, vec<nd> p) {
  auto result = std::make_pair(p, std::numeric_limits<num_t>::max());
  for (auto&& i : points) {
    if (i == p) { continue; }
    auto d = distance(p, i);
    if (d < result.second) { result = std::make_pair(i, d); }
  }
  return result.first;
}

}  // namespace nearest_neighbor
