#pragma once
/// \file types.hpp
#include <ndtree/utility/compact_optional.hpp>
#define NDTREE_64_BIT_TYPES

namespace ndtree {
inline namespace v1 {
//

#if !defined(NDTREE_32_BIT_TYPES) || !defined(NDTREE_64_BIT_TYPES)
using int_t = int_fast32_t;
using uint_t = uint_fast32_t;
using num_t = double;
#elif defined(NDTREE_32_BIT_TYPES)
using int_t = int32_t;
using uint_t = uint32_t;
using num_t = float;
#elif defined(NDTREE_64_BIT_TYPES)
using int_t = int64_t;
using uint_t = uint64_t;
using num_t = double;
#endif

constexpr int_t operator"" _i(unsigned long long int i) {
  return static_cast<int_t>(i);
}

constexpr uint_t operator"" _u(unsigned long long int i) {
  return static_cast<uint_t>(i);
}

/// Index of a node within a tree
using node_idx
 = compact_optional<empty_scalar_value<uint_t, std::numeric_limits<int>::max()>,
                    struct node_idx_tag>;

constexpr node_idx operator"" _n(unsigned long long int i) {
  return node_idx{static_cast<uint_t>(i)};
}

/// Index of a sibling group within a tree
using siblings_idx
 = compact_optional<empty_scalar_value<uint_t, std::numeric_limits<int>::max()>,
                    struct siblings_tag>;

constexpr siblings_idx operator"" _sg(unsigned long long int i) {
  return siblings_idx{static_cast<uint_t>(i)};
}

/// nd-tree
template <int nd> struct tree;

/// Child position range
template <typename Tree> using child_pos = typename Tree::child_pos;

template <class T> using get_tag_t = typename T::tag;

/// Neighbor tags
struct same_level_tag {};
struct child_level_tag {};

}  // namespace v1
}  // namespace ndtree
