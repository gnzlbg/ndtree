#pragma once
/// \file geohash.hpp

#include <array>
#include <type_traits>
#include <htree/utility/assert.hpp>
#include <htree/utility/math.hpp>

namespace htree {

/// Position of a node within its parent node
/// in Morton Z-Curve order
template <int nd> struct position_in_parent {
  constexpr static bool in_bounds(unsigned int value) noexcept {
    return value >= 0 and value < math::ipow(2, nd);
  }
  static_assert(nd > 0 and nd <= 3, "unsupported dimension for geohash piece");

  /// Set the hash value
  constexpr position_in_parent& operator=(unsigned int value) noexcept {
    HTREE_ASSERT(in_bounds(value), "");
    p = value;
  }
  /// Get the hashed value
  constexpr unsigned int operator*() const noexcept { return p; }

 private:
  /// For nd = 1: [0, 2)
  /// For nd = 2: [0, 4)
  /// For nd = 3: [0, 8)
  unsigned int p : nd;
};

template <int nd> using pip = position_in_parent<nd>;

HTREE_ASSERT_STANDARD_LAYOUT(pip<1>);
HTREE_ASSERT_STANDARD_LAYOUT(pip<2>);
HTREE_ASSERT_STANDARD_LAYOUT(pip<3>);

/// Geohash
template <int nd, int max_no_levels = 5> struct geohash {
  std::array<pip<nd>, max_no_levels> value;
};

HTREE_ASSERT_STANDARD_LAYOUT(geohash<1>);
HTREE_ASSERT_STANDARD_LAYOUT(geohash<2>);
HTREE_ASSERT_STANDARD_LAYOUT(geohash<3>);

/// A hash with a traversal from the root node to a leaf node
template <int nd, int max_no_levels = 5>
struct root_to_leaf_hash : geohash<nd, max_no_levels> {};

/// A hash with a traversal from a leaf node to the root node
template <int nd, int max_no_levels = 5>
struct leaft_to_root_hash : geohash<nd, max_no_levels> {};

/// A hash with a traversal from the root to a node at a given level
template <int nd, int max_no_levels = 5>
struct root_to_node_hash : geohash<nd, max_no_levels> {
  char node_level;
};

/// A hash with a traversal from a node at a given level to the root node
template <int nd, int max_no_levels = 5>
struct node_to_root_hash : geohash<nd, max_no_levels> {
  char node_level;
};

}  // namespace htree
