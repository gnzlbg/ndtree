#pragma once
/// \file stack_vector.hpp Stack allocated vector
#include <type_traits>
#include <ndtree/utility/ranges.hpp>

namespace ndtree {
inline namespace v1 {
//

/// Vector with stack storage
///
/// TODO: this is far from good (but it works for me TM)
/// - make it actually zero size for capacity == 0
/// - constexpr constructor/destructor for trivially default
///   constructible and trivially destructible types?
/// - implement the rest of the std::vector interface
/// - make it as exception safe as possible
/// - document undefined behavior
template <class T, std::size_t Capacity>  //
struct stack_vector {
 private:
  std::aligned_storage_t<sizeof(T), std::alignment_of<T>::value>
   data_[Capacity != 0 ? Capacity : 1];  // TODO: hack!
  std::size_t size_ = 0;

  static_assert(std::is_nothrow_destructible<T>{},
                "stack_vector requires T to be nothrow destructible");

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

  /// Is the vector empty?
  constexpr bool empty() const noexcept { return size() == size_type(0); }

  /// Maximum number of elements that can be allocated in the vector
  ///
  /// Note: just to be compatible with std::vector
  constexpr size_type max_size() const noexcept { return capacity(); }

  /// Maximum number of elements that can be allocated in the vector
  constexpr size_type capacity() const noexcept { return Capacity; }

  /// Pointer to the vector's data
  constexpr T* data() noexcept { return reinterpret_cast<T*>(data_); }

  /// Pointer to the vector's data
  constexpr const T* data() const noexcept {
    return reinterpret_cast<T const*>(data_);
  }

  /// Begin iterator
  constexpr iterator begin() noexcept { return data(); }
  /// Begin iterator
  constexpr const_iterator begin() const noexcept { return data(); }
  /// Begin const iterator
  constexpr const_iterator cbegin() const noexcept { return begin(); }

  /// End iterator
  constexpr iterator end() noexcept { return data() + size(); }
  /// End iterator
  constexpr const_iterator end() const noexcept { return data() + size(); }
  /// End const iterator
  constexpr const_iterator cend() const noexcept { return end(); }

 private:
  /// Destroys all elements of the vector inplace
  /// (required because of using placement new to construct the elements
  /// on the std::aligned_storage)
  CONCEPT_REQUIRES(!std::is_trivially_destructible<T>{})
  constexpr void destroy_all() noexcept {
    for (auto it = begin(), e = end(); it != e; ++it) { it->~T(); }
  }
  /// For trivially destructible types there is nothing to do
  ///
  /// TODO: move the storage and destructors to a base class
  /// and omit the destructor for trivially destructible types: this would
  /// allow the compiler to automatically generate a constexpr destructor
  CONCEPT_REQUIRES(std::is_trivially_destructible<T>{})
  constexpr void destroy_all() {}

  using nothrow_move_constructible = std::is_nothrow_move_constructible<T>;
  using nothrow_copy_constructible = std::is_nothrow_copy_constructible<T>;
  using nothrow_move_assignable = std::is_nothrow_move_assignable<T>;
  using nothrow_copy_assignable = std::is_nothrow_copy_assignable<T>;

 public:
  ~stack_vector() noexcept { destroy_all(); }

 private:
  /// A range can be assigned from if it's an InputRange and T can either be
  /// MoveConstructed or CopyConstructed from the range's elemenets
  ///
  /// TODO: update this to get the ranges value type, and check that T can be
  /// Move/Copy constructed from the range's elements
  template <typename Rng>
  using RangeAssignable
   = meta::fast_and<InputRange<Rng>,
                    meta::fast_or<MoveConstructible<T>, CopyConstructible<T>>>;

  /// Assign from range: if the range is not an rvalue, always copy assign
  ///
  /// TODO: underconstrained, see RangeAssignable's todo
  template <class Rng, CONCEPT_REQUIRES_(RangeAssignable<Rng>{}
                                         and CopyConstructible<T>{})>
  constexpr auto range_assign(Rng&& rng) -> std::
   enable_if_t<!std::is_rvalue_reference<decltype(std::forward<Rng>(rng))>{}> {
    for (auto&& i : rng) { emplace_back(i); }
  }
  /// Assign from range: if the range is an rvalue, always move assign
  ///
  /// TODO: underconstrained, see RangeAssignable's todo
  template <class Rng, CONCEPT_REQUIRES_(RangeAssignable<Rng>{}
                                         and MoveConstructible<T>{})>
  constexpr auto range_assign(Rng&& rng) -> std::
   enable_if_t<std::is_rvalue_reference<decltype(std::forward<Rng>(rng))>{}> {
    for (auto&& i : rng) { emplace_back(std::move(i)); }
  }

 public:
  /// Constructors/Assignment
  ///@{

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

  /// Range constructor
  template <class Rng, CONCEPT_REQUIRES_(RangeAssignable<Rng>{})>
  constexpr stack_vector(Rng&& rng) {
    range_assign(std::forward<Rng>(rng));
  }

  /// TODO: range assignment

  constexpr stack_vector(std::initializer_list<T> init) {
    range_assign(std::move(init));
  }

  /// Construction from Iterator/Sentinel pair
  template <class It, class S,
            CONCEPT_REQUIRES_(InputIterator<It>{} and IteratorRange<It, S>{}
                              and RangeAssignable<range<It, S>>{})>
  constexpr stack_vector(It&& it, S&& s)
   : stack_vector(range<It, S>{std::forward<It>(it), std::forward<S>(s)}) {}

  ///@}

  /// \name Element access
  ///@{

  constexpr reference operator[](size_type pos) noexcept {
    NDTREE_ASSERT(pos < size(), "index {} ouf-of-bounds [0,{})", pos, size());
    return *(begin() + pos);
  };
  constexpr const_reference operator[](size_type pos) const noexcept {
    NDTREE_ASSERT(pos < size(), "index {} ouf-of-bounds [0,{})", pos, size());
    return *(begin() + pos);
  }

  /// TODO: implement throwing at(size_type pos) -> reference
  /// TODO: implement throwing at(size_type pos) const -> const_reference

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

  ///@}

  /// Erase elements in range [first, last)
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

  /// Clears the vector
  constexpr void clear() noexcept {
    destroy_all();
    size_ = size_type(0);
  }

  /// Emplace back
  ///
  /// TODO: placement new is not constexpr so this cannot ever possibly be
  /// constexpr
  template <class... Args, CONCEPT_REQUIRES_(Constructible<T, Args...>{})>
  void emplace_back(Args&&... args) {
    NDTREE_ASSERT(size() < capacity(),
                  "cannot emplace back element, vector is full");
    new (data_ + size_) T(std::forward<Args>(args)...);
    ++size_;
  }

  /// Push back
  ///
  /// TODO: Cannot be constexpr because of emplace_back usage of placement new
  template <class U, CONCEPT_REQUIRES_(Constructible<T, U>{})>
  void push_back(U&& value) {
    emplace_back(std::forward<U>(value));
  }

  /// Pop back
  ///
  /// TODO: Cannot be constexpr because it explicitly calls a destructor
  /// (overload for trivially destructible types and make it constexpr)
  void pop_back() noexcept {
    back()->~T();
    --size_;
  }

  /// Resize
  ///
  /// TODO: Assert size before hand
  /// TODO: cannot be constexpr because of empalce back and erase
  /// TODO: exception safety
  void resize(size_type count, T const& value) {
    if (count > size()) {
      while (count != size()) { emplace_back(value); }
    } else {
      erase(end() - (size_ - count), end());
    }
  }

  /// Resize
  ///
  /// TODO: Assert size before hand
  /// TODO: cannot be constexpr because of empalce back and erase
  /// TODO: exception safety
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
