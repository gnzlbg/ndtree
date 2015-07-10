#pragma once
/// \file tagged_sentinel_optional.hpp
///
/// TODO: relax RandomIncrementable concepts
///
#include <limits>
#include <string>
#include <htree/utility/ranges.hpp>

namespace htree {

/// Tagged optional with a sentinel value
template <class T, class Tag = void, T sentinel = std::numeric_limits<T>::max()>
struct tagged_sentinel_optional {
  static_assert(Regular<T>(), "T must be a regular type");
  T value = sentinel;

 private:
  using this_t = tagged_sentinel_optional;

 public:
  constexpr T operator*() const noexcept {
    assert(*this);
    return value;
  }
  constexpr T& operator*() noexcept {
    assert(*this);
    return value;
  }
  constexpr explicit operator bool() const noexcept {
    return value != sentinel;
  }

  template <class U, class V, V s,
            class Other = tagged_sentinel_optional<U, V, s>>
  constexpr explicit operator Other() const noexcept {
    return Other{value};
  }
  constexpr bool operator==(this_t const& o) const noexcept {
    return value == o.value;
  }
  constexpr bool operator!=(this_t const& o) const noexcept {
    return value != o.value;
  }

  CONCEPT_REQUIRES(WeaklyOrdered<T>())
  constexpr bool operator<(this_t const& o) const noexcept {
    return value < o.value;
  }
  CONCEPT_REQUIRES(WeaklyOrdered<T>())
  constexpr bool operator>(this_t const& o) const noexcept {
    return value > o.value;
  }
  CONCEPT_REQUIRES(WeaklyOrdered<T>())
  constexpr bool operator<=(this_t const& o) const noexcept {
    return value <= o.value;
  }
  CONCEPT_REQUIRES(WeaklyOrdered<T>())
  constexpr bool operator>=(this_t const& o) const noexcept {
    return value >= o.value;
  }

  CONCEPT_REQUIRES(RandomAccessIncrementable<T>())
  static constexpr auto rng(T const& from, T const& to) noexcept {
    return view::iota(from, to)
           | view::transform([](T const& t) { return this_t{t}; });
  }

  CONCEPT_REQUIRES(RandomAccessIncrementable<T>())
  static constexpr auto rng(this_t const& from, this_t const& to) noexcept {
    return rng(*from, *to);
  }

  template <class OStream>
  friend constexpr OStream& operator<<(OStream& os, this_t const& a) {
    os << *a;
    return os;
  };

  CONCEPT_REQUIRES(RandomAccessIncrementable<T>())
  this_t& operator++() noexcept {
    ++(*this);
    return (*this);
  }

  CONCEPT_REQUIRES(RandomAccessIncrementable<T>())
  this_t operator++(int) noexcept {
    this_t tmp(*this);
    ++(*this);
    return tmp;
  }



  CONCEPT_REQUIRES(RandomAccessIncrementable<T>())
  friend this_t operator+(this_t const& other) noexcept {
    return this_t{(*this) + (*other)};
  }
};

}  // namespace htree
