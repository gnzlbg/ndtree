#pragma once
/// \file default.hpp Default location
//#include <ndtree/location/fast.hpp>
#include <ndtree/location/slim.hpp>
namespace ndtree {
inline namespace v1 {
//

namespace location {

// template <uint_t nd, typename T = uint_t> using default_location = fast<nd,
// T>;

template <uint_t nd, typename T = uint_t> using default_location = slim<nd, T>;

}  // namespace location

}  // namespace v1

}  // namespace ndtree
