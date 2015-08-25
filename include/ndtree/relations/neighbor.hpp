#pragma once
/// \file neighbor.hpp Neighbor relationships
#include <array>
#include <ndtree/tree.hpp>
#include <ndtree/types.hpp>
#include <ndtree/relations/tree.hpp>
#include <ndtree/utility/assert.hpp>
#include <ndtree/utility/math.hpp>
#include <ndtree/utility/bounded.hpp>
#include <ndtree/utility/terminate.hpp>

namespace ndtree {
inline namespace v1 {
//

/// \name Neighbor relations
///@{

/// Number of node neighbors at the node level (same level)
///
/// \param nd [in] spatial dimension of the node
/// \param m [in] spatial dimension of the face between node and neighbors
///
/// Formula: \f$\ 2^{n_d - m} \begin{pmatrix} n_d // m \end{pmatrix} \f$
///
static constexpr uint_t no_neighbors(uint_t nd, uint_t m,
                                     same_level_tag) noexcept {
  return no_faces(nd, m);
}

/// Number of node neighors at the children level of a node (node level + 1)
///
/// \param nd [in] spatial dimension of the node
/// \param m [in] spatial dimension of the face
///
/// Formula: number of nodes sharing a face * number of neighbors
///
static constexpr uint_t no_neighbors(uint_t nd, uint_t m, child_level_tag) {
  return no_nodes_sharing_face(nd, m) * no_neighbors(nd, m, same_level_tag{});
}

/// \name Stencils of neighbor children sharing a m dimensional face with a node
/// of dimension nd
///@{

template <int nd, int m> struct neighbor_children_sharing_face {
  std::array<std::array<child_pos<tree<nd>>, 0>, 0> stencil;
};

/// One dimensional stencil across faces
///
///  |-- Left neighbor --|-- Node --|-- Right neighbor --|
///  |         | child 1 |          | child 0  |         |
///
template <> struct neighbor_children_sharing_face<1, 0> {
  std::array<std::array<child_pos<tree<1>>, 1>, 2> stencil{{{{1}}, {{0}}}};
};

template <> struct neighbor_children_sharing_face<2, 1> {
  std::array<std::array<child_pos<tree<2>>, 2>, 4> stencil{
   {{{1, 3}}, {{0, 2}}, {{2, 3}}, {{0, 1}}}};
};

template <> struct neighbor_children_sharing_face<2, 0> {
  std::array<std::array<child_pos<tree<2>>, 1>, 4> stencil{
   {{{3}}, {{2}}, {{1}}, {{0}}

   }};
};

template <> struct neighbor_children_sharing_face<3, 2> {
  std::array<std::array<child_pos<tree<3>>, 4>, 6> stencil{{{{1, 3, 5, 7}},
                                                            {{0, 2, 4, 6}},
                                                            {{2, 3, 6, 7}},
                                                            {{0, 1, 4, 5}},
                                                            {{4, 5, 6, 7}},
                                                            {{0, 1, 2, 3}}}};
};

template <> struct neighbor_children_sharing_face<3, 1> {
  std::array<std::array<child_pos<tree<3>>, 2>, 12> stencil{{{{3, 7}},
                                                             {{2, 6}},
                                                             {{1, 5}},
                                                             {{0, 4}},
                                                             {{5, 7}},
                                                             {{4, 6}},
                                                             {{6, 7}},
                                                             {{4, 5}},
                                                             {{1, 3}},
                                                             {{0, 2}},
                                                             {{2, 3}},
                                                             {{0, 1}}}};
};

template <> struct neighbor_children_sharing_face<3, 0> {
  std::array<std::array<child_pos<tree<3>>, 1>, 8> stencil{
   {{{7}}, {{6}}, {{5}}, {{4}}, {{3}}, {{2}}, {{1}}, {{0}}}};
};

///@}  // Neighbor children sharing face stencils

template <int nd> using neighbor_offset = std::array<int_t, nd>;

/// TODO: benchmark the following vs look-up table based versions

template <int nd, int m> struct manifold_neighbors;

/// Neighbor of an nd-dimensional node across a (nd - m)-dimensional face
template <int nd, int m> struct manifold_neighbors {
  static_assert(nd >= 0 and nd <= 3, "");

  static constexpr uint_t dimension() noexcept { return nd; }

  static constexpr uint_t size() noexcept {
    return no_neighbors(nd, nd - m, same_level_tag{});
  }

  using neighbor_idx = bounded<uint_t, 0_u, size(), manifold_neighbors<nd, m>>;

  auto operator()() const noexcept { return neighbor_idx::rng(); }

  /// Offset of location code
  constexpr neighbor_offset<nd> operator[](neighbor_idx i) const noexcept {
    NDTREE_ASSERT(i, "");
    const auto n = *i;
    neighbor_offset<nd> o;
    switch (m) {
      case 1: {  // nd - 1
        for (auto&& d : dimensions(nd)) {
          o[d] = (n / 2) == d % nd ? (n % 2 ? 1 : -1) : 0;
        }
        return o;
      }
      case 2: {  // nd - 2
        if (n < 4) {
          for (auto&& d : dimensions(nd)) {
            o[d] = (n / math::ipow(2_u, d)) % 2 ? 1 : -1;
          }
          if (nd == 3) { o[2] = 0; }
        } else {
          const uint_t c = (n / 2) % 2;
          const uint_t v = n % 2 == 0 ? -1 : 1;
          o[0] = (!c) * v;
          o[1] = c * v;
          o[2] = (n - 4) / 4 == 0 ? -1 : 1;
        }
        return o;
      }
      case 3: {  // nd - 3
        for (auto&& d : dimensions(nd)) {
          o[d] = (n / math::ipow(2_u, d)) % 2 ? 1 : -1;
        }
        return o;
      }
      default: { NDTREE_TERMINATE("unimplemented"); }
    }
  }

  static constexpr auto children_sharing_face(neighbor_idx i) noexcept {
    using ncsf = neighbor_children_sharing_face<nd, nd - m>;
    auto stencil = ncsf{}.stencil;
    NDTREE_ASSERT(ranges::size(stencil) != 0_u,
                  "error: neighbor_idx {} | nd {} | m {}", *i, nd, m);
    return stencil[*i];
  }

  static constexpr auto no_same_level_neighbors() noexcept {
    return no_neighbors(nd, m, same_level_tag{});
  }
  static constexpr auto no_child_level_neighbors() noexcept {
    return no_neighbors(nd, m, child_level_tag{});
  }
};

template <typename T> using neighbor_idx_t = typename T::neighbor_idx;

template <int nd> using face_neighbors = manifold_neighbors<nd, 1>;
template <int nd> using edge_neighbors = manifold_neighbors<nd, 2>;
template <int nd> using corner_neighbors = manifold_neighbors<nd, 3>;

/// neighbor of a nd-dimensional node across an m dimensional surface
template <int nd, int m>
using surface_neighbors =
 // points:
 meta::
  if_c<m == 1, meta::if_c<nd == 1, face_neighbors<1>,
                          meta::if_c<nd == 2, edge_neighbors<2>,
                                     meta::if_c<nd == 3, corner_neighbors<3>,
                                                meta::nil_>>>,
       // lines:
       meta::
        if_c<m == 2,
             meta::if_c<nd == 2, face_neighbors<2>,
                        meta::if_c<nd == 3, edge_neighbors<3>, meta::nil_>>,
             // planes:
             meta::if_c<m == 3 and nd == 3, face_neighbors<3>, meta::nil_>>>;

struct neighbor_idx {
  uint_t value;
  template <class T, T from, T to, class Tag>
  operator bounded<T, from, to, Tag>() const noexcept {
    return bounded<T, from, to, Tag>{value};
  }
};

constexpr neighbor_idx operator"" _ni(unsigned long long int i) {
  return neighbor_idx{static_cast<uint_t>(i)};
}

constexpr auto max_no_neighbors(int nd) {
  int s = 0;
  for (int i = 1; i != nd + 1; ++i) {
    s += no_neighbors(nd, i, child_level_tag{});
  }
  return s;
}

///@} Neighbor relations

}  // namespace v1
}  // namespace ndtree
