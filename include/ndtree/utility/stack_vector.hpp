#pragma once
/// \file vector.hpp Stack allocated vector
#include <type_traits>
#include <ndtree/utility/ranges.hpp>

namespace ndtree {
inline namespace v1 {
//

/// Vector with stack storage
///
/// Implementation far from complete. TODO:
/// - make zero size for capacity == 0
/// - constexpr constructor/destructor for trivially default
///   constructible and trivially destructible types?
template <class T, std::size_t Capacity>  //
struct stack_vector {
 private:
  std::aligned_storage_t<sizeof(T), std::alignment_of<T>::value>
   data_[Capacity != 0 ? Capacity : 1];  // hack!
  std::size_t size_ = 0;

 public:
  using value_type = T;
  using size_type = std::size_t;
  using difference_type = std::ptrdiff_t;
  using reference = T&;
  using const_reference = T const&;
  using pointer = T*;
  using const_pointer = T const*;
  using iterator = T*;
  using const_iterator = T const*;

  /// Size of the vector
  constexpr size_type size() const noexcept { return size_; }

  /// pointer to the vector's data
  constexpr T* data() noexcept { return reinterpret_cast<T*>(data_); }
  constexpr const T* data() const noexcept {
    return reinterpret_cast<T const*>(data_);
  }

  constexpr iterator begin() noexcept { return data(); }
  constexpr const_iterator begin() const noexcept { return data(); }

  constexpr iterator end() noexcept { return data() + size(); }
  constexpr const_iterator end() const noexcept { return data() + size(); }

 private:
  // stack_vector requires T to be nothrow destructible
  CONCEPT_REQUIRES(!std::is_trivially_destructible<T>{})
  constexpr void destroy_all() noexcept {
    for (auto it = begin(), e = end(); it != e; ++it) { it->~T(); }
  }
  CONCEPT_REQUIRES(std::is_trivially_destructible<T>{})
  constexpr void destroy_all() {}

  using nothrow_move_constructible = std::is_nothrow_move_constructible<T>;
  using nothrow_copy_constructible = std::is_nothrow_copy_constructible<T>;
  using nothrow_move_assignable = std::is_nothrow_move_assignable<T>;
  using nothrow_copy_assignable = std::is_nothrow_copy_assignable<T>;

 public:
  ~stack_vector() noexcept { destroy_all(); }

 private:
  template <typename Rng>
  using RangeAssignable
   = meta::fast_and<InputRange<Rng>,
                    meta::fast_or<MoveConstructible<T>, CopyConstructible<T>>>;

  template <class Rng, CONCEPT_REQUIRES_(RangeAssignable<Rng>{}
                                         and CopyConstructible<T>{})>
  constexpr auto range_assign(Rng&& rng) -> std::
   enable_if_t<!std::is_rvalue_reference<decltype(std::forward<Rng>(rng))>{}> {
    for (auto&& i : rng) { emplace_back(i); }
  }
  template <class Rng, CONCEPT_REQUIRES_(RangeAssignable<Rng>{}
                                         and MoveConstructible<T>{})>
  constexpr auto range_assign(Rng&& rng) -> std::
   enable_if_t<std::is_rvalue_reference<decltype(std::forward<Rng>(rng))>{}> {
    for (auto&& i : rng) { emplace_back(std::move(i)); }
  }

 public:
  constexpr stack_vector() = default;
  constexpr stack_vector(stack_vector const& other) noexcept(
   nothrow_copy_constructible{}) {
    range_assign(other);
  };
  constexpr stack_vector(stack_vector&& other) noexcept(
   nothrow_move_constructible{}) {
    range_assign(std::move(other));
  };
  constexpr stack_vector& operator=(stack_vector const& other) noexcept(
   nothrow_copy_assignable{}) {
    clear();
    range_assign(other);
    return *this;
  }

  constexpr stack_vector& operator=(stack_vector&& other) noexcept(
   nothrow_move_assignable{}) {
    clear();
    range_assign(std::move(other));
    return *this;
  }

  constexpr stack_vector(size_type count) noexcept(
   nothrow_move_constructible{}) {
    while (count != 0) {
      emplace_back(T{});
      --count;
    }
  }

  constexpr stack_vector(size_type count, T const& value) noexcept(
   nothrow_copy_constructible{}) {
    while (count != 0) {
      emplace_back(value);
      --count;
    }
  }

  template <class Rng, CONCEPT_REQUIRES_(RangeAssignable<Rng>{})>
  constexpr stack_vector(Rng&& rng) {
    range_assign(std::forward<Rng>(rng));
  }
  constexpr stack_vector(std::initializer_list<T> init) {
    range_assign(std::move(init));
  }

  template <class It, class S,
            CONCEPT_REQUIRES_(InputIterator<It>{} and IteratorRange<It, S>{}
                              and RangeAssignable<range<It, S>>{})>
  constexpr stack_vector(It&& it, S&& s)
   : stack_vector(range<It, S>{std::forward<It>(it), std::forward<S>(s)}) {}

  constexpr reference operator[](size_type pos) noexcept {
    NDTREE_ASSERT(pos < size(), "index {} ouf-of-bounds [0,{})", pos, size());
    return *(begin() + pos);
  };
  constexpr const_reference operator[](size_type pos) const noexcept {
    NDTREE_ASSERT(pos < size(), "index {} ouf-of-bounds [0,{})", pos, size());
    return *(begin() + pos);
  }

  constexpr reference front() noexcept {
    NDTREE_ASSERT(size() > 0, "calling front on an empty vector");
    return *begin();
  }
  constexpr const_reference front() const noexcept {
    NDTREE_ASSERT(size() > 0, "calling front on an empty vector");
    return *begin();
  }

  constexpr reference back() noexcept {
    NDTREE_ASSERT(size() > 0, "calling back on an empty vector");
    return *(end() - 1);
  }
  constexpr const_reference back() const noexcept {
    NDTREE_ASSERT(size() > 0, "calling back on an empty vector");
    return *(end() - 1);
  }

  constexpr const_iterator cbegin() const noexcept { return begin(); }
  constexpr const_iterator cend() const noexcept { return end(); }

  constexpr bool empty() const noexcept { return size() == size_type(0); }

  constexpr size_type max_size() const noexcept { return capacity(); }

  constexpr size_type capacity() const noexcept { return Capacity; }

  iterator erase(const_iterator first, const_iterator last) {
    iterator f = begin() + (first - begin());
    iterator l = begin() + (last - begin());
    if (last != end()) {
      for (iterator i = l + 1, e = end(); i != e; ++i) {
        ranges::swap(*i, *(f++));
      }
    }
    size_ -= end() - f;
    for (; f != l; ++f) { f->~T(); }
    return end();
  }

 public:
  constexpr void clear() noexcept {
    destroy_all();
    size_ = size_type(0);
  }

  template <class... Args, CONCEPT_REQUIRES_(Constructible<T, Args...>{})>
  constexpr void emplace_back(Args&&... args) {
    NDTREE_ASSERT(size() < capacity(),
                  "cannot emplace back element, vector is full");
    new (data_ + size_) T(std::forward<Args>(args)...);
    ++size_;
  }

  template <class U, CONCEPT_REQUIRES_(Constructible<T, U>{})>
  constexpr void push_back(U&& value) {
    emplace_back(std::forward<U>(value));
  }

  void pop_back() noexcept {
    back()->~T();
    --size_;
  }

  void resize(size_type count, T const& value) {
    if (count > size()) {
      while (count != size()) { emplace_back(value); }
    } else {
      erase(end() - (size_ - count), end());
    }
  }

  void resize(size_type count) {
    if (count > size()) {
      while (count != size()) { emplace_back(T{}); }
    } else {
      erase(end() - (size_ - count), end());
    }
  }
};

}  // namespace v1
}  // namespace ndtree
