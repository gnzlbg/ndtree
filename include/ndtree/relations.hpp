#pragma once
/// \file relations.hpp
#include <array>
#include <ndtree/types.hpp>
#include <ndtree/utility/assert.hpp>
#include <ndtree/utility/math.hpp>
#include <ndtree/utility/bounded.hpp>
#include <ndtree/utility/terminate.hpp>

namespace ndtree {
inline namespace v1 {
//

/// Range of spatial dimensions: [0, nd)
///
/// TODO: make constexpr when view::iota is constexpr
auto dimensions(uint_t nd) noexcept { return view::iota(0_u, nd); }
NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(dimensions(1));
NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(dimensions(2));
NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(dimensions(3));

/// Number of children of a node
///
/// \param nd [in] spatial dimension of the node
///
/// Formula: \f$ 2^{nd} \f$
///
static constexpr uint_t no_children(uint_t nd) noexcept {
  return math::ipow(2_u, nd);
}

/// Number of siblings of a node
///
/// Siblings are nodes sharing the same parent.
///
/// \param nd [in] spatial dimension of the node
///
/// Formula: \f$ 2^{nd} \f$
///
static constexpr uint_t no_siblings(uint_t nd) noexcept {
  return no_children(nd);
}

/// Number of nodes sharing a single face within a node
///
/// \param nd [in] spatial dimension of the node
/// \param m [in] spatial dimension of a face
///
/// Formula: \f$ 2 ^ {m} \f$ for \f$ m >= m \f$ otherwise 0.
///
static constexpr uint_t no_nodes_sharing_face(uint_t nd, uint_t m) noexcept {
  return (nd >= m) ? math::ipow(2_u, m) : 0_u;
}

/// Number of nodes sharing a single face at a given level
///
/// \param nd [in] spatial dimension of the nodes
/// \param m [in] spatial dimension of the face
/// \param level [in] level starting from the root node (which is at level 0)
///
/// Formula: \f$ (2^{m})^{l} \f$
///
static constexpr uint_t no_nodes_sharing_face_at_level(uint_t nd, uint_t m,
                                                       uint_t level) noexcept {
  const auto nsf = no_nodes_sharing_face(nd, m);
  return nsf == 0_u and level == 0_u ? 0_u : math::ipow(nsf, level);
}

/// Number of node faces
///
/// \param nd [in] spatial dimension of the node
/// \param m [in] spatial dimension of the face
///
/// Formula: \f$\ 2^{n_d - m} \begin{pmatrix} n_d // m \end{pmatrix} \f$
///
static constexpr uint_t no_faces(uint_t nd, uint_t m) noexcept {
  return m <= nd ? math::ipow(2_u, nd - m) * math::binomial_coefficient(nd, m)
                 : 0_u;
}

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

template <int nd> using neighbor_offset = std::array<int_t, nd>;

/// TODO: benchmark the following vs look-up table based versions

template <int nd, int m> struct manifold_neighbors;

/// Neighbor of an nd-dimensional node acros a (nd - m)-dimensional face
template <int nd, int m> struct manifold_neighbors {
  static constexpr uint_t size() noexcept {
    return no_neighbors(nd, nd - m, same_level_tag{});
  }

  using neighbor_idx = bounded<uint_t, 0_u, size(), manifold_neighbors<nd, m>>;

  auto operator()() const noexcept { return neighbor_idx::rng(); }

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

// /// (nd - 1) - neighbor offsets
// template <int nd>  //
// struct face_neighbors {
//   static constexpr uint_t size() noexcept {
//     return no_neighbors(nd, nd - 1, same_level_tag{});
//   }
//   static constexpr neighbor_offset<nd> offset(uint_t n) {
//     NDTREE_ASSERT(n < size(), "");
//     neighbor_offset<nd> o;
//     for (auto&& d : dimensions(nd)) {
//       o[d] = (n / 2) == d % nd ? (n % 2 ? 1 : -1) : 0;
//     }
//     return o;
//   }
// };

// template <int nd> struct corner_neighbors {
//   static constexpr uint_t size() noexcept {
//     return no_neighbors(nd, nd - 3, same_level_tag{});
//   }
//   static constexpr neighbor_offset<nd> offset(uint_t n) {
//     NDTREE_ASSERT(n < size(), "");
//     NDTREE_ASSERT(nd > 2, "");
//     neighbor_offset<nd> o;
//     for (auto&& d : dimensions(nd)) {
//       o[d] = (n / math::ipow(2_u, d)) % 2 ? 1 : -1;
//     }
//     return o;
//   }
// };

// template <int nd> struct edge_neighbors {
//   static constexpr uint_t size() noexcept {
//     return no_neighbors(nd, nd - 2, same_level_tag{});
//   }
//   static constexpr neighbor_offset<nd> offset(uint_t n) {
//     NDTREE_ASSERT(n < size(), "");
//     NDTREE_ASSERT(nd > 1, "");
//     static_assert(nd >= 0 and nd <= 3, "unimplemented");
//     neighbor_offset<nd> o;

//     if (n < 4) {
//       for (auto&& d : dimensions(nd)) {
//         o[d] = (n / math::ipow(2_u, d)) % 2 ? 1 : -1;
//       }
//       if (nd == 3) { o[2] = 0; }
//     } else {
//       const uint_t c = (n / 2) % 2;
//       const uint_t v = n % 2 == 0 ? -1 : 1;
//       o[0] = (!c) * v;
//       o[1] = c * v;
//       o[2] = (n - 4) / 4 == 0 ? -1 : 1;
//     }
//     return o;
//   }
// };

// template <int nd> constexpr auto point = meta::size_t<0>{};
// template <int nd> constexpr auto edge = meta::size_t<1>{};
// template <int nd> constexpr auto face = meta::size_t<2>{};

}  // namespace v1
}  // namespace ndtree
