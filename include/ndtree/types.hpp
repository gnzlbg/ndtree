#pragma once
/// \file types.hpp
#include <ndtree/utility/compact_optional.hpp>

namespace ndtree {

inline namespace v1 {
/// Index of a node within a tree
using node_idx
 = compact_optional<empty_scalar_value<int, std::numeric_limits<int>::max()>,
                    struct node_idx_tag>;

constexpr node_idx operator"" _n(unsigned long long int i) {
  return node_idx{static_cast<int>(i)};
}

using siblings_idx
 = compact_optional<empty_scalar_value<int, std::numeric_limits<int>::max()>,
                    struct siblings_tag>;

constexpr siblings_idx operator"" _sg(unsigned long long int i) {
  return siblings_idx{static_cast<int>(i)};
}

}  // namespace v1

}  // namespace ndtree
