#pragma once
/// \file fast.hpp
#include <array>
#include <ndtree/types.hpp>
#include <ndtree/relations/tree.hpp>
#include <ndtree/utility/bit.hpp>
#include <ndtree/utility/math.hpp>
#include <ndtree/utility/ranges.hpp>
#include <ndtree/utility/compact_optional.hpp>

namespace ndtree {
inline namespace v1 {
//

namespace location {

/// Location within a tree of dimension nd
///
/// The underlying storage type T determines the maximum level
/// within the tree that can be represented.
///
/// TODO: reconsider the storage format
/// TODO: make level a member function
/// TODO: make x and level private
/// TODO: rework optional location
/// TODO: check overflow from array of floats
///
/// Right now optimized for insertion but makes it a pain for shifting
/// coordinates and doing useful stuff with the location hashes
///
template <uint_t nd, typename T = uint_t> struct fast {
  using this_t = fast<nd, T>;
  using opt_this_t = compact_optional<this_t>;
  using value_type = this_t;
  using storage_type = this_t;
  using reference_type = this_t const&;

  std::array<T, nd> x;
  uint_t level_ = 0;

  constexpr uint_t level() const noexcept { return level_; }

  static constexpr auto dimension() noexcept { return nd; }
  static constexpr auto dimensions() noexcept {
    return ndtree::dimensions(dimension());
  }

  static constexpr uint_t max_level() noexcept {
    constexpr auto max = 8 * sizeof(T{});
    return max * nd > 64 ? 64 / nd - 1 : max - 1;
  }

  static constexpr uint_t no_levels() noexcept { return max_level() + 1; }

  constexpr auto levels() const noexcept {
    return level() == 0 ? view::iota(0_u, 0_u) : view::iota(1_u, level() + 1_u);
  }

  void reset_bits() noexcept {
    for (auto&& d : dimensions()) { x[d] = static_cast<T>(0); }
  }

  fast() = default;
  fast(fast const&) = default;
  fast& operator=(fast const&) = default;
  fast(fast&&) = default;
  fast& operator=(fast&&) = default;

  template <typename U, CONCEPT_REQUIRES_(std::is_floating_point<U>{})>
  fast(std::array<U, nd> x_, uint_t l = max_level())
   : level_(l) {
    NDTREE_ASSERT(l <= max_level(), "");

    for (auto&& d : dimensions()) {
      NDTREE_ASSERT(x_[d] > 0. and x_[d] < 1., "location from non-normalized "
                                               "float (d: {}, x[d]: {}) "
                                               "out-of-range (0., 1.)",
                    d, x_[d]);
    }

    num_t scale = math::ipow(2_u, l);
    for (auto&& d : dimensions()) { from_int_r(d, x_[d] * scale); }
  }

  fast(std::initializer_list<uint_t> ps) {
    reset_bits();
    for (auto&& p : ps) { push(p); }
  }

  // from root:
  template <class Rng, CONCEPT_REQUIRES_(Range<Rng>())> fast(Rng&& ps) {
    reset_bits();
    for (auto&& p : ps) { push(p); }
  }

  void push(uint_t pos_in_parent) {
    ++level_;
    NDTREE_ASSERT(level() <= max_level(), "");
    for (auto&& d : dimensions()) {
      bit::set(x[d], level(), bit::get(pos_in_parent, d));
    }
  }

  uint_t operator[](uint_t l) const noexcept {
    NDTREE_ASSERT(l > 0 and l <= level(), "");
    uint_t value = 0;
    for (auto&& d : dimensions()) { bit::set(value, d, bit::get(x[d], l)); }
    return value;
  }

  auto operator()() const noexcept {
    return (level() == 0_u ? view::iota(0_u, 0_u)
                           : view::iota(1_u, level() + 1_u))
           | view::transform([&](uint_t l) { return (*this)[l]; });
  }

  /// Reverses the bits of the location
  constexpr void reverse() noexcept {
    if (level() == 0 || level() == 1) { return; }
    int_t first = 1;
    int_t last = level();
    int swap_count = 0;
    while (last - first > 0) {
      for (auto&& d : dimensions()) { bit::swap(x[d], first, last); }
      ++first;
      --last;
      swap_count++;
    }
  }

  T to_int(uint_t d) const noexcept {
    return bit::to_int_r(x[d], static_cast<T>(1), static_cast<T>(level() + 1));
  }

  explicit operator std::array<uint_t, nd>() const noexcept {
    std::array<uint_t, nd> result;
    for (auto&& d : dimensions()) { result[d] = to_int(d); }
    return result;
  }

  explicit operator uint_t() const noexcept {
    std::array<uint_t, nd> tmp(*this);
    tmp[0] += math::ipow(2_u, level());
    uint_t result = 0;
    std::size_t i = 0;
    for (auto l : view::iota(0_u, level() + 1_u)) {
      for (auto d : dimensions()) {
        bit::set(result, i, bit::get(tmp[d], l));
        ++i;
      }
    }
    return result;
  }

  void from_int_r(uint_t d, uint_t v) noexcept {
    uint_t i = 1;
    for (auto l : levels() | view::reverse) {
      bit::set(x[d], i, bit::get(v, l - 1));
      ++i;
    }
  }

  friend opt_this_t shift(this_t l, std::array<int_t, nd> offset) noexcept {
    for (auto&& d : dimensions()) {
      if (bit::overflows_on_add(l.to_int(d), offset[d], l.level())) {
        return opt_this_t{};
      }
    }
    for (auto&& d : dimensions()) {
      l.x[d] = bit::to_int_r(static_cast<T>(l.to_int(d) + offset[d]),
                             static_cast<T>(0), static_cast<T>(l.level() + 1));
    }
    return opt_this_t{l};
  }

  uint_t pop() noexcept {
    uint_t tmp = (*this)[level()];
    --level_;
    return tmp;
  }

  static constexpr this_t empty_value() noexcept {
    this_t t;
    t.level_ = std::numeric_limits<uint_t>::max();
    return t;
  }
  static constexpr bool is_empty_value(this_t v) noexcept {
    return v.level() == std::numeric_limits<uint_t>::max();
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
OStream& operator<<(OStream& os, fast<nd, T> const& lc) {
  os << "[id: " << static_cast<uint_t>(lc) << ", lvl: " << lc.level()
     << ", xs: {";
  std::array<uint_t, nd> xs(lc);
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
constexpr bool operator==(fast<nd, T> const& a, fast<nd, T> const& b) {
  return a.level() == 0 ? a.level() == b.level()
                        : a.level() == b.level() and equal(a(), b());
}

template <uint_t nd, typename T>
constexpr bool operator!=(fast<nd, T> const& a, fast<nd, T> const& b) {
  return !(a == b);
}

template <uint_t nd, typename T>
constexpr bool operator<(fast<nd, T> const& a, fast<nd, T> const& b) {
  return static_cast<uint_t>(a) < static_cast<uint_t>(b);
}

template <uint_t nd, typename T>
constexpr bool operator<=(fast<nd, T> const& a, fast<nd, T> const& b) {
  return (a == b) or (a < b);
}

template <uint_t nd, typename T>
constexpr bool operator>(fast<nd, T> const& a, fast<nd, T> const& b) {
  return !(a <= b);
}

template <uint_t nd, typename T>
constexpr bool operator>=(fast<nd, T> const& a, fast<nd, T> const& b) {
  return !(a < b);
}

}  // namespace location

}  // namespace v1
}  // namespace ndtree
