#pragma once
/// \file concepts.hpp
#include <array>
#include <ndtree/types.hpp>
#include <ndtree/utility/ranges.hpp>
#include <ndtree/utility/compact_optional.hpp>

namespace ndtree {
inline namespace v1 {
//

namespace concepts {
namespace rc = ranges::concepts;

struct Dimensioned {
  template <typename T>
  auto requires_(T&& t) -> decltype(
   rc::valid_expr(rc::convertible_to<uint_t>(t.dimension()),           //
                  rc::model_of<rc::RandomAccessRange>(t.dimensions())  //
                  ));
};

/// Locational codes model the location concept
struct Location : rc::refines<rc::Regular, Dimensioned, rc::TotallyOrdered> {
  template <typename T>  //
  auto requires_(T&& t) -> decltype(
   rc::valid_expr((t.push(0_u), 42),  //
                  rc::convertible_to<uint_t>(t.pop()),
                  rc::convertible_to<uint_t>(t.max_level()),        //
                  rc::convertible_to<uint_t>(t.no_levels()),        //
                  rc::convertible_to<uint_t>(t.level()),            //
                  (t.reverse(), 42),                                //
                  rc::model_of<rc::RandomAccessRange>(t()),         //
                  rc::convertible_to<uint_t>(t[uint_t{}]),          //
                  rc::model_of<rc::RandomAccessRange>(t.levels()),  //
                  (T(std::initializer_list<uint_t>{0_u}), 42),      //
                  (T(std::vector<uint_t>{0_u, 0_u}), 42),           //
                  (compact_optional<T>{}, 42),                      //
                  rc::convertible_to<compact_optional<T>>(shift(
                   t, std::array<int_t, rc::uncvref_t<T>::dimension()>{}))));
};

}  // namespace concepts

template <typename T>
using Dimensioned
 = concepts::rc::models<concepts::Dimensioned, ranges::uncvref_t<T>>;

template <typename T>
using Location = concepts::rc::models<concepts::Location, ranges::uncvref_t<T>>;

}  // namespace v1
}  // namespace ndtree
