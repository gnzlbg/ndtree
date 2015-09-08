#pragma once
/// \file nearest_neighbor.hpp Nearest neighbor container
#include <utility>
#include <vector>
#include <unordered_map>
#include <ndtree/tree.hpp>
#include <ndtree/algorithm.hpp>
#include <ndtree/utility/stack_vector.hpp>
#include "nearest_neighbor.hpp"

namespace nearest_neighbor {

namespace faster {

template <uint_t nd>  //
struct points {
 private:
  static constexpr uint_t k = 9;
  using points_t = stack_vector<vec<nd>, k>;
  using point_idx
   = compact_optional<empty_scalar_value<uint_t,
                                         std::numeric_limits<int>::max()>,
                      struct point_idx_tag>;

  tree<nd> tree_;
  std::vector<points_t> points_;

 public:
  points(uint_t capacity) : tree_(capacity), points_(capacity) {}

 private:
  void redistribute_points_to_children(node_idx n) noexcept {
    balanced_refine(tree_, n, [&](node_idx parent) {
      if (points_[*parent].size() == 0) { return; }

      for (auto&& pi : points_[*parent]) {
        auto child_node
         = node_or_parent_at(tree_, location::default_location<nd>(pi)).idx;
        points_[*child_node].push_back(pi);
      }
      points_[*parent].clear();
    });
  }

 public:
  void push(vec<nd> p) noexcept {
    auto n = node_or_parent_at(tree_, location::default_location<nd>(p)).idx;
    if (points_[*n].size() < k) {
      points_[*n].push_back(p);
    } else {
      redistribute_points_to_children(n);
      push(p);
    }
  }

  vec<nd> nearest_neighbor(vec<nd> const& p) const noexcept {
    auto result = std::make_pair(p, std::numeric_limits<num_t>::max());
    auto update = [&](auto&& o) {
      auto dist = distance(p, o);
      if (dist < result.second) { result = std::make_pair(o, dist); }
    };

    auto n = node_or_parent_at(tree_, location::default_location<nd>(p)).idx;
    for (auto&& i : points_[*n]) {
      if (i == p) { continue; }
      update(i);
    }

    for (auto&& neighbor : node_neighbors(tree_, n)) {
      for (auto&& i : points_[*neighbor]) { update(i); }
    }
    return result.first;
  }

  void sort() noexcept {
    dfs_sort(tree_, [&](node_idx a, node_idx b) {
      ranges::swap(points_[*a], points_[*b]);
    });
  }
};

}  // namespace faster

}  // namespace nearest_neighbor
