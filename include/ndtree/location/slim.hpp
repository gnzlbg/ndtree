#pragma once
/// \file slim.hpp
#include <ndtree/relations/dimension.hpp>
#include <ndtree/relations/tree.hpp>
#include <ndtree/types.hpp>
#include <ndtree/utility/assert.hpp>
#include <ndtree/utility/bit.hpp>

namespace ndtree {
inline namespace v1 {
namespace location {

template <uint_t nd, typename UInt = uint_t>  //
struct slim {
  using this_t = slim<nd, UInt>;

  using value_type = this_t;
  using storage_type = this_t;
  using reference_type = this_t const&;

  static_assert(UnsignedIntegral<UInt>{},
                "location::slim storage must be an unsigned integral type");

  UInt value = 1;  /// Default constructed to the root node

  static constexpr uint_t dimension() noexcept { return nd; }
  static auto dimensions() noexcept { return ndtree::dimensions(dimension()); }

  static constexpr uint_t no_levels() noexcept {
    constexpr auto no_bits = bit::width<UInt>;
    constexpr auto loc_size = nd;
    return math::floor((static_cast<double>(no_bits - loc_size))
                       / static_cast<double>(loc_size));
  }

  static constexpr uint_t max_level() noexcept { return no_levels() - 1; }

  constexpr uint_t level() const noexcept {
    NDTREE_ASSERT(
     value, "trying to obtain the level of an uninitialized location code");

#if defined(__GCC__) || defined(__clang__)
    // This:
    // {
    //   auto key = value;
    //   for (int d = 0; key; ++d) {
    //     if (key == 1) { return d; }
    //     key >>=3;
    //   }
    //   return key;
    // }
    // is equivalent to:
    return (bit::width<UInt> - 1 - bit::clz(value)) / nd;
#else
#pragma message "error unsuported compiler"
#endif
  }

  void push(uint_t position_in_parent) noexcept {
    NDTREE_ASSERT(position_in_parent < no_children(nd),
                  "position in parent {} out-of-bounds [0, {}) (nd: {})",
                  position_in_parent, no_children(nd), nd);
    NDTREE_ASSERT(level() != max_level(),
                  "location \"full\": level equals max_level {}", max_level());
    value = (value << nd) + position_in_parent;
  }
  template <typename Tag>
  void push(bounded<uint_t, 0, no_children(nd), Tag> position_in_parent) {
    push(*position_in_parent);
  }
  uint_t pop() noexcept {
    NDTREE_ASSERT(level() > 0_u, "cannot pop root-node from location code");
    uint_t tmp = (*this)[level()];
    value >>= nd;
    return tmp;
  }

  bool valid() const noexcept { return value != 0; }

  uint_t operator[](const uint_t level_) const noexcept {
    NDTREE_ASSERT(level_ > 0 and level_ <= level(),
                  "level {} out-of-bounds [1, {})", level_, level());
    UInt mask = no_children(nd) - 1;
    const UInt mask_shift = (level() - level_) * nd;
    mask <<= mask_shift;
    mask &= value;
    mask >>= mask_shift;
    return mask;
  }

  auto levels() const noexcept { return view::iota(1_u, level() + 1_u); }

  auto operator()() const noexcept {
    const auto l = level();
    return (l == 0 ? view::iota(0_u, 0_u) : levels())
           | view::transform([=](uint_t l_i) { return (*this)[l_i]; });
  }

  slim() = default;
  slim(slim const&) = default;
  slim& operator=(slim const&) = default;
  slim(slim&&) = default;
  slim& operator=(slim&&) = default;

  slim(std::initializer_list<uint_t> list) : slim() {
    for (auto&& p : list) { push(p); }
  }

  static constexpr std::array<UInt, nd> set_level(std::array<UInt, nd> xs,
                                                  uint_t l) noexcept {
    bit::set(xs[0], l, true);
    return xs;
  }

  static constexpr UInt encode(std::array<UInt, nd> xs, uint_t l) noexcept {
    return bit::morton::encode(set_level(xs, l));
  }

  static constexpr std::array<UInt, nd> remove_level(std::array<UInt, nd> xs,
                                                     uint_t l) noexcept {
    bit::set(xs[0], l, false);
    return xs;
  }

  static constexpr std::array<UInt, nd> decode(UInt v, uint_t l) noexcept {
    return remove_level(bit::morton::decode(v, std::array<UInt, nd>{}), l);
  }

  template <typename U, CONCEPT_REQUIRES_(std::is_floating_point<U>{})>
  slim(std::array<U, nd> x_, uint_t l = (max_level() - 1)) {
    NDTREE_ASSERT(l < max_level(), "");

    for (auto&& d : dimensions()) {
      NDTREE_ASSERT(x_[d] > 0. and x_[d] < 1., "location from non-normalized "
                                               "float (d: {}, x[d]: {}) "
                                               "out-of-range (0., 1.)",
                    d, x_[d]);
    }

    num_t scale = math::ipow(2_u, l);
    std::array<UInt, nd> tmp;
    for (auto&& d : dimensions()) { tmp[d] = x_[d] * scale; }
    value = encode(tmp, l);

    while (level() > l) { pop(); }
  }

  // from root:
  template <class Rng, CONCEPT_REQUIRES_(Range<Rng>())>
  slim(Rng&& ps) : slim() {
    for (auto&& p : ps) { push(p); }
  }

  void reverse() {
    slim other;
    for (auto l : (*this)() | view::reverse) { other.push(l); }
    (*this) = other;
  }

  explicit operator uint_t() const noexcept { return value; }

  explicit operator std::array<UInt, nd>() const noexcept {
    return decode(value, level());
  }

  static constexpr this_t empty_value() noexcept {
    this_t t;
    t.value = UInt{0};
    return t;
  }
  static constexpr bool is_empty_value(this_t v) noexcept {
    return v.value == UInt{0};
  }

  static constexpr value_type const& access_value(
   storage_type const& v) noexcept {
    return v;
  }
  static constexpr value_type const& store_value(value_type const& v) noexcept {
    return v;
  }
  static constexpr value_type&& store_value(value_type&& v) noexcept {
    return std::move(v);
  }
};

template <typename OStream, uint_t nd, typename T>
OStream& operator<<(OStream& os, slim<nd, T> const& lc) {
  os << "[id: " << static_cast<uint_t>(lc) << ", lvl: " << lc.level()
     << ", xs: {";
  std::array<T, nd> xs(lc);
  for (auto&& d : dimensions(nd)) {
    os << xs[d];
    if (d != nd - 1) { os << ", "; }
  }
  os << "}, pip: {";
  uint_t counter = 0;
  for (auto&& pip : lc()) {
    counter++;
    os << pip;
    if (counter != lc.level()) { os << ","; }
  }
  os << "}]";
  return os;
}

template <uint_t nd, typename T>
compact_optional<slim<nd, T>> shift(slim<nd, T> t,
                                    std::array<int_t, nd> offset) noexcept {
  using sl = slim<nd, T>;
  auto lvl = t.level();
  auto xs = sl::decode(t.value, lvl);
  if (none_of(dimensions(nd), [&](auto&& d) {
        return bit::overflows_on_add(xs[d], offset[d], lvl);
      })) {
    for (auto&& d : dimensions(nd)) { xs[d] += offset[d]; }
    t.value = sl::encode(xs, lvl);
    NDTREE_ASSERT(t.value != 0_u, "logic error, encoding delivers zero");
    return compact_optional<sl>{t};
  }
  return compact_optional<sl>{};
}

template <uint_t nd, class T>
constexpr bool operator==(slim<nd, T> const& a, slim<nd, T> const& b) noexcept {
  return a.value == b.value;
}

template <uint_t nd, class T>
constexpr bool operator!=(slim<nd, T> const& a, slim<nd, T> const& b) noexcept {
  return !(a == b);
}

template <uint_t nd, class T>
constexpr bool operator<(slim<nd, T> const& a, slim<nd, T> const& b) noexcept {
  return a.value < b.value;
}

template <uint_t nd, class T>
constexpr bool operator<=(slim<nd, T> const& a, slim<nd, T> const& b) noexcept {
  return (a == b) or (a < b);
}

template <uint_t nd, class T>
constexpr bool operator>(slim<nd, T> const& a, slim<nd, T> const& b) noexcept {
  return !(a.value <= b.value);
}

template <uint_t nd, class T>
constexpr bool operator>=(slim<nd, T> const& a, slim<nd, T> const& b) noexcept {
  return !(a < b);
}

static_assert(std::is_standard_layout<slim<1_u>>{}, "");
static_assert(std::is_literal_type<slim<1_u>>{}, "");
static_assert(std::is_nothrow_constructible<slim<1_u>>{}, "");
static_assert(std::is_nothrow_default_constructible<slim<1_u>>{}, "");
static_assert(std::is_trivially_copy_constructible<slim<1_u>>{}, "");
static_assert(std::is_nothrow_copy_constructible<slim<1_u>>{}, "");
static_assert(std::is_trivially_move_constructible<slim<1_u>>{}, "");
static_assert(std::is_nothrow_move_constructible<slim<1_u>>{}, "");
static_assert(std::is_trivially_assignable<slim<1_u>, slim<1_u>>{}, "");
static_assert(std::is_nothrow_assignable<slim<1_u>, slim<1_u>>{}, "");
static_assert(std::is_trivially_copy_assignable<slim<1_u>>{}, "");
static_assert(std::is_nothrow_copy_assignable<slim<1_u>>{}, "");
static_assert(std::is_trivially_move_assignable<slim<1_u>>{}, "");
static_assert(std::is_nothrow_move_assignable<slim<1_u>>{}, "");
static_assert(std::is_trivially_destructible<slim<1_u>>{}, "");
static_assert(std::is_nothrow_destructible<slim<1_u>>{}, "");

}  // namespace location
}  // namespace v1
}  // namespace ndtree
