#pragma once
/// \file dfs_sort.hpp
#include <ndtree/types.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
//

struct dfs_sort_fn {
 private:
  /// Sorts the sub-tree spanned by sibling group \s in depth-first order, with
  /// the siblings of each group sorted in Morton Z-Curve order.
  ///
  /// Runtime complexity: O(N), where N is the number of nodes in the tree.
  /// Space complexity: O(log(N)) stack frames.
  ///
  /// \param s [in] Start sibling group. All nodes below it will be sorted
  ///
  /// \pre \p s is at its correct position
  ///
  /// The algorithm is recursive. It assumes that the input group \p s is at its
  /// correct position in memory. It then traverses the children groups of its
  /// siblings in deapth-first order. If the children group of a sibling is not
  /// at its correct position, it is swapped with the group at the correct
  /// position.
  ///
  /// \post is_compact() && is_sorted()
  template <typename Tree>
  static siblings_idx sort_impl(Tree& t, siblings_idx s) noexcept {
    siblings_idx should = s;
    for (auto n : t.nodes(s) | t.with_children()) {
      ++should;

      siblings_idx c_sg = t.children_group(n);

      if (c_sg != should) {
        t.swap(c_sg, should);
        should = sort_impl(t, should);
      } else {
        should = sort_impl(t, c_sg);
      }
    }

    return should;
  }

 public:
  /// Sorts the tree in depth-first order, with the siblings of each group
  /// sorted in Morton Z-Curve order
  ///
  /// Runtime complexity: O(N), where N is the number of nodes in the tree.
  /// Space complexity: O(log(N)) stack frames.
  ///
  /// \post is_compact() && is_sorted()
  template <typename Tree> void operator()(Tree& t) const noexcept {
    sort_impl(t, 0_sg);
    t.set_first_free_sibling_group(t.sibling_group(t.size()));
    NDTREE_ASSERT(t.is_compact(), "the tree must be compact after sorting");
  }
};

namespace {
constexpr auto&& dfs_sort = static_const<dfs_sort_fn>::value;
}

}  // namespace v1
}  // namespace ndtree
