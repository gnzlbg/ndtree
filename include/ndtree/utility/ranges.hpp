#pragma once
/// \file ranges.hpp Range utilities
#include <range/v3/all.hpp>

namespace ndtree {
inline namespace v1 {
using namespace ranges;

template <typename Box, typename Int,
          typename ValueType = typename Box::value_type,
          CONCEPT_REQUIRES_(Same<Int, ValueType>{}
                            and RandomAccessIncrementable<ValueType>{})>
auto boxed_ints(Int const& from, Int const& to) {
  return view::iota(from, to)
         | view::transform([](ValueType const& v) -> Box { return v; });
}

template <typename Box, typename ValueType = typename Box::value_type,
          CONCEPT_REQUIRES_(RandomAccessIncrementable<ValueType>{})>
auto boxed_ints(Box const& from, Box const& to) {
  return boxed_ints<Box>(*from, *to);
}

}  // namespace v1
}  // namespace ndtree

#define NDTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(Value)  \
  static_assert(::ndtree::RandomAccessRange<decltype(Value)>{} \
                 && ::ndtree::SizedRange<decltype(Value)>{},   \
                "The value " #Value " is not a random access sized range!")
