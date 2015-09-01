#pragma once
/// \file dfs_sort.hpp
#include <ndtree/types.hpp>
#include <ndtree/utility/static_const.hpp>

namespace ndtree {
inline namespace v1 {
//

struct dfs_sort_fn {
 private:
  /// Swaps data of each node between two sibling groups
  ///
  /// \param t         [in] Tree to be sorted
  /// \param a         [in] Sibling groups to swap with \p b
  /// \param b         [in] Sibling groups to swap with \p a
  /// \param data_swap [in] Function (node, node) -> ignored that swaps data
  ///                       between two nodes.
  template <typename Tree, typename DataSwap>
  static void swap_data(Tree& t, siblings_idx a, siblings_idx b,
                        DataSwap&& data_swap) noexcept {
    RANGES_FOR(auto&& s, ranges::view::zip(t.nodes(a), t.nodes(b))) {
      data_swap(get<0>(s), get<1>(s));
    }
  }

  /// Sorts the sub-tree spanned by sibling group \s in depth-first order, with
  /// the siblings of each group sorted in Morton Z-Curve order.
  ///
  /// Runtime complexity: O(N), where N is the number of nodes in the tree.
  /// Space complexity: O(log(N)) stack frames.
  ///
  /// \param t [in] Tree to be sorted.
  /// \param s [in] Start sibling group. All nodes below it will be sorted
  /// \param data_swap [in] Function (node, node) -> ignored that swaps data
  ///                       between two nodes.
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
  template <typename Tree, typename DataSwap>
  static siblings_idx sort_impl(Tree& t, siblings_idx s,
                                DataSwap&& data_swap) noexcept {
    siblings_idx should = s;
    for (auto n : t.nodes(s) | t.with_children()) {
      ++should;

      siblings_idx c_sg = t.children_group(n);

      if (c_sg != should) {
        t.swap(c_sg, should);
        swap_data(t, c_sg, should, data_swap);
        should = sort_impl(t, should, data_swap);
      } else {
        should = sort_impl(t, c_sg, data_swap);
      }
    }

    return should;
  }

  struct binary_fn_t {
    template <typename A, typename B>
    void operator()(A&&, B&&) const noexcept {}
  };

 public:
  /// Sorts the tree in depth-first order, with the siblings of each group
  /// sorted in Morton Z-Curve order
  ///
  /// \param t [in] Tree to be sorted
  /// \param data_swap [in] Function (node, node) -> ignored that swaps data
  ///                       between two tree nodes.
  ///
  /// Runtime complexity: O(N), where N is the number of nodes in the tree.
  /// Space complexity: O(log(N)) stack frames.
  ///
  /// \post is_compact() && is_sorted()
  template <typename Tree, typename DataSwap = binary_fn_t>
  void operator()(Tree& t, DataSwap&& data_swap = binary_fn_t{}) const
   noexcept {
    sort_impl(t, 0_sg, std::forward<DataSwap>(data_swap));
    t.set_first_free_sibling_group(t.sibling_group(t.size()));
    NDTREE_ASSERT(t.is_compact(), "the tree must be compact after sorting");
  }
};

namespace {
constexpr auto&& dfs_sort = static_const<dfs_sort_fn>::value;
}

}  // namespace v1
}  // namespace ndtree
