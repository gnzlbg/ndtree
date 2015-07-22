#pragma once
/// \file static_const.hpp
//
// Adapted from the range-v3 library with the following copyright and license:
//
// Range v3 library
//
//  Copyright Eric Niebler 2013-2014
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/ericniebler/range-v3
//

namespace ndtree {
inline namespace v1 {
template <typename T> struct static_const { static constexpr T value{}; };
template <typename T> constexpr T static_const<T>::value;
}  // namespace v1
}  // namespace ndtree
