#pragma once
/// \file dimension.hpp
#include <ndtree/types.hpp>
#include <ndtree/utility/assert.hpp>
#include <ndtree/utility/ranges.hpp>

namespace ndtree {
inline namespace v1 {
/// Range of spatial dimensions: [0, nd)
///
/// TODO: make constexpr when view::iota is constexpr
auto dimensions(uint_t nd) noexcept { return view::iota(0_u, nd); }
NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(dimensions(1));
NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(dimensions(2));
NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(dimensions(3));

}  // namespace v1
}  // namespace ndtree
