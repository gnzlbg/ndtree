#pragma once
/// \file nearest_neighbor.hpp
#include <utility>
#include <vector>
#include <unordered_map>
#include <ndtree/tree.hpp>
#include <ndtree/algorithm.hpp>
#include <ndtree/location.hpp>
#include <ndtree/utility/stack_vector.hpp>
// TODO: rename to nearest neighbor search

namespace nearest_neighbor {

template <int nd> using vec = std::array<double, nd>;

template <int nd> struct points {
  static constexpr std::size_t k = 9;
  std::vector<vec<nd>> points_;
  ndtree::tree<nd> tree_;
  std::vector<ndtree::node_idx> node_idx_;
  std::unordered_map<ndtree::node_idx, ndtree::stack_vector<std::size_t, k>>
   point_idx_;

  points(std::size_t capacity)
   : tree_(capacity * 2), node_idx_(capacity), point_idx_(capacity) {
    points_.reserve(capacity);
  }

  auto balanced_refine(ndtree::node_idx n) {
    auto l = ndtree::node_level(tree_, n);
    for (auto n_i : ndtree::node_neighbors(tree_, n)) {
      auto n_l = ndtree::node_level(tree_, n_i);
      if (n_l == l - 1 and tree_.is_leaf(n_i)) {
        redistribute_to_children(n_i);
      }
    }
    return tree_.refine(n);
  }

  void sort() {
    // first: sort the tree
    auto swap_data = [&](ndtree::node_idx a, ndtree::node_idx b) {
      ranges::swap(point_idx_[a], point_idx_[b]);
      for (auto p : point_idx_[a]) { node_idx_[p] = a; }
      for (auto p : point_idx_[b]) { node_idx_[p] = b; }
    };
    ndtree::dfs_sort(tree_, swap_data);

    // second: sort the particles following tree order
    std::size_t should = 0;
    for (auto n : tree_.nodes() | tree_.leaf()) {
      for (auto&& p : point_idx_[n]) {
        if (p != should) {
          auto&& should_ps = point_idx_[node_idx_[should]];
          for (auto&& sp : should_ps) {
            if (sp == should) { sp = p; }
          }
          ranges::swap(points_[p], points_[should]);
          ranges::swap(node_idx_[p], node_idx_[should]);

          p = should;
        }
        ++should;
      }
    }
    if (should != points_.size()) {
      std::cout << "error size differes" << std::endl;
    }

    // third: shrink unordered map
    for (auto it = ranges::begin(point_idx_); it != ranges::end(point_idx_);) {
      if (it->second.size() == std::size_t{0}) {
        it = point_idx_.erase(it);
      } else {
        ++it;
      }
    }
  }

  void redistribute_to_children(ndtree::node_idx n) {
    auto b = balanced_refine(n);
    NDTREE_ASSERT(b, "refining node {} failed", n);
    if (point_idx_.count(n) == 0) { return; }
    auto ps = point_idx_[n];
    auto child_level = ndtree::node_level(tree_, n) + 1;
    for (auto&& pi : ps) {
      auto l_i = ndtree::location<nd>(points_[pi], child_level);
      auto n_i = ndtree::node_at(tree_, l_i);
      NDTREE_ASSERT(n_i, "invalid node??");
      node_idx_[pi] = n_i;
      if (point_idx_.count(n_i) > 0) {
        point_idx_[n_i].push_back(pi);
      } else {
        point_idx_[n_i] = ndtree::stack_vector<std::size_t, k>{};
        point_idx_[n_i].push_back(pi);
      }
    }
    point_idx_.erase(n);
  }

  void tree_insert(std::size_t idx) {
    auto l = ndtree::location<nd>(points_[idx]);
    auto n = ndtree::node_or_parent_at(tree_, l).idx;
    if (point_idx_.count(n) == 0 || point_idx_[n].size() < k) {
      point_idx_[n].push_back(idx);
      node_idx_[idx] = n;
      return;
    } else {
      // redistribute points to children leafs
      redistribute_to_children(n);
      tree_insert(idx);
    }
  }

  void push(vec<nd> p) {
    auto idx = points_.size();
    points_.push_back(p);
    tree_insert(idx);
  }

  auto idx() const {
    return ranges::view::iota(std::size_t{0}, points_.size());
  }

  static ndtree::num_t distance(vec<nd> a, vec<nd> b) noexcept {
    ndtree::num_t d = 0;
    RANGES_FOR(auto&& p, ranges::view::zip(a, b)) {
      d += std::pow(std::get<0>(p) - std::get<1>(p), 2);
    }
    return std::sqrt(d);
  }

  std::size_t nearest_neighbor(std::size_t idx) {
    auto n = node_idx_[idx];
    std::pair<std::size_t, ndtree::num_t> result
     = std::make_pair(idx, std::numeric_limits<ndtree::num_t>::max());

    auto update = [&](auto&& a, auto&& b) {
      auto dist = distance(points_[a], points_[b]);
      if (dist < result.second) { result = std::make_pair(b, dist); }
    };

    // compute distance with other points in the tree node
    for (auto i : point_idx_[n]) {
      if (i == idx) { continue; }
      update(idx, i);
    }

    auto neighbors = ndtree::node_neighbors(tree_, n);
    for (auto neighbor : neighbors) {
      if (point_idx_.count(neighbor) == 0) { continue; }
      auto neighbor_point_ids = point_idx_[neighbor];
      for (auto neighbor_point : neighbor_point_ids) {
        update(idx, neighbor_point);
      }
    }
    return result.first;
  }

  std::size_t nn_brute_force(std::size_t i) {
    auto result = std::make_pair(i, std::numeric_limits<ndtree::num_t>::max());
    for (auto j : idx()) {
      if (i == j) { continue; }
      auto d = distance(points_[i], points_[j]);
      if (d < result.second) { result = std::make_pair(j, d); }
    }
    return result.first;
  }
};

}  // namespace nearest_neighbor
