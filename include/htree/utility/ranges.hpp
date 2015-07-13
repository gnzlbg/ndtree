#pragma once
/// \file ranges.hpp Range utilities
#include <range/v3/all.hpp>

namespace htree {
using namespace ranges;
}  // namespace htree

#define HTREE_STATIC_ASSERT_RANDOM_ACCESS_SIZED_RANGE(Value)  \
  static_assert(::htree::RandomAccessRange<decltype(Value)>{} \
                 && ::htree::SizedRange<decltype(Value)>{},   \
                "The value " #Value " is not a random access sized range!")
