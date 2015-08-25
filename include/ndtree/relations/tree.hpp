#pragma once
/// \file relations.hpp Tree relationships.
#include <array>
#include <ndtree/types.hpp>
#include <ndtree/utility/assert.hpp>
#include <ndtree/utility/math.hpp>
#include <ndtree/utility/bounded.hpp>
#include <ndtree/utility/terminate.hpp>

namespace ndtree {
inline namespace v1 {
//

/// \name Tree relations
///@{

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

///@} // Tree relations

}  // namespace v1
}  // namespace ndtree
