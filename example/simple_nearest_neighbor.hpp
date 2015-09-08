#include "nearest_neighbor.hpp"

namespace nearest_neighbor {

namespace simple {

/// This is a container to store the points:
/// Internally it is going to use an octree graph,
/// and a std::unordered_map for storing the points themselves
template <uint_t nd>  //
struct points {
 private:
  /// Internally we need two things:
  tree<nd> tree_;  ///< The octree graph

  /// k is the maximum number of nodes that we are going to store per tree node
  static constexpr uint_t k = 9;

  /// For each leaf tree node we store a stack allocated vector
  /// that can hold k nodes in a simple std::unordered_map:
  std::unordered_map<node_idx, stack_vector<vec<nd>, k>> points_;

 public:
  /// The container will have a fixed capacity
  /// The tree has more nodes than leaf nodes so 2*capacity is a valid but
  /// very conservative estimate
  points(uint_t capacity) : tree_(capacity * 2), points_(1.5 * capacity) {}

  /// Push adds a point to the container
  void push(vec<nd> p) {
    /// This finds the leaf node of the tree that contains the point:
    auto n = node_or_parent_at(tree_, location::default_location<nd>(p)).idx;
    /// If the point fits in the node, we append it
    if (points_.count(n) == 0 || points_[n].size() < k) {
      points_[n].push_back(p);
    } else {
      /// Otherwise we redistribute the points of that node into child nodes
      redistribute_points_to_children(n);
      /// and try again:
      push(p);
    }
  }

  /// Redistributes the points of node n into its children
  void redistribute_points_to_children(node_idx n) {
    /// We refine the nodes maintaining the tree balanced
    /// The balance_refine algorithm takes a projection from a parent node
    /// to its children that it uses when nodes are refined
    balanced_refine(tree_, n, [&](node_idx parent) {
      /// If the parent node has no points there is nothing to do
      if (points_.count(parent) == 0) { return; }
      /// Otherwise append its points to the new leaf nodes
      for (auto&& pi : points_[parent]) {
        auto child
         = node_or_parent_at(tree_, location::default_location<nd>(pi)).idx;
        points_[child].push_back(pi);
      }
      /// And delete them from the parent node
      points_.erase(parent);
    });
  }

  /// The nearest neighbor search algorithm
  vec<nd> nearest_neighbor(vec<nd>& p) {
    /// We need to store the nearest neighbor as well as its distance from
    /// the point
    auto result = std::make_pair(p, std::numeric_limits<num_t>::max());
    /// And use this function to update the result
    auto update = [&](auto&& o) {
      /// Compute the distance between the point p and another points o
      /// and if its smaller than the current one store the point and the
      /// distance in the result:
      auto dist = distance(p, o);
      if (dist < result.second) { result = std::make_pair(o, dist); }
    };

    /// Find the leaf node containing the point
    auto n = node_or_parent_at(tree_, location::default_location<nd>(p)).idx;

    /// Update result with other points within the same tree node
    for (auto&& i : points_[n]) {
      if (i == p) { continue; }
      update(i);
    }

    /// Update result with other points in adjacent tree nodes:
    for (auto&& neighbor : node_neighbors(tree_, n)) {
      if (points_.count(neighbor) == 0) { continue; }
      for (auto&& i : points_[neighbor]) { update(i); }
    }
    return result.first;
  }

  /// This shrinks the container by eliminating entris within the map that have
  /// zero points
  void shrink() {
    for (auto it = ranges::begin(points_); it != ranges::end(points_);) {
      if (it->second.size() == uint_t{0}) {
        it = points_.erase(it);
      } else {
        ++it;
      }
    }
  }

  //// Sorts the container in DFS/Z-Morton curve order
  void sort() {
    /// This is used to swap the nodes data within dfs_sort:
    auto swap_data
     = [&](node_idx a, node_idx b) { ranges::swap(points_[a], points_[b]); };
    /// Sort in DFS/Z-morton order:
    dfs_sort(tree_, swap_data);
    /// Shrink the container:
    shrink();
  }
};

}  // namespace simple

}  // namespace nearest_neighbor
