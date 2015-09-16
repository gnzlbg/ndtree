#pragma once
/// \file assert.hpp Assert macro
#include <type_traits>
#include <ndtree/utility/at.hpp>
#include <ndtree/utility/likely.hpp>
#include <ndtree/utility/fmt.hpp>

// This requires the gnu/clang zero-variadic-macro-arguments language extension
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

/// Tells the optimizer that it can assume that the condition is true
#define NDTREE_ASSUME(cond)               \
  do {                                    \
    if (!(cond)) __builtin_unreachable(); \
  } while (0)

/// Asserts a condition in DEBUG mode. Useful for checking
/// pre/postconditions and invariants.
///
/// \warning if NDTREE_DISABLE_ASSERTIONS is defined the condition in the if
/// clause is always false and NDTREE_ASSERT compiles to nothing! Still this
/// happens after the compiler has verified the correctness of the NDTREE_ASSERT
/// code.
#ifndef NDTREE_DISABLE_ASSERTIONS
#define NDTREE_ASSERT_AT(condition, message, AT, ...)                      \
  do {                                                                     \
    if (NDTREE_UNLIKELY(!(condition))) {                                   \
      ::ndtree::fmt::print(                                                \
       stderr,                                                             \
       "\nASSERTION FAILED:\n\n  condition: \"{}\"\n  message: \"" message \
       "\"\n\n",                                                           \
       #condition, ##__VA_ARGS__);                                         \
      NDTREE_PRINT_AT(AT);                                                 \
      __builtin_debugtrap();                                               \
    }                                                                      \
    NDTREE_ASSUME((condition));                                            \
  } while (false)
#else  // NDTREE_DISABLE_ASSERTIONS
#define NDTREE_ASSERT_AT(condition, message, AT, ...)                      \
  do {                                                                     \
    if (false && NDTREE_UNLIKELY(!(condition))) {                          \
      ::ndtree::fmt::print(                                                \
       stderr,                                                             \
       "\nASSERTION FAILED:\n\n  condition: \"{}\"\n  message: \"" message \
       "\"\n",                                                             \
       #condition, ##__VA_ARGS__);                                         \
      NDTREE_PRINT_AT(AT);                                                 \
      __builtin_debugtrap();                                               \
    }                                                                      \
    NDTREE_ASSUME((condition));                                            \
  } while (false)
#endif  // NDTREE_DISABLE_ASSERTIONS

#define NDTREE_ASSERT(condition, message, ...) \
  NDTREE_ASSERT_AT(condition, message, NDTREE_AT_, ##__VA_ARGS__)

#pragma clang diagnostic pop

#define NDTREE_ASSERT_STANDARD_LAYOUT(T)                           \
  static_assert(std::is_standard_layout<T>{}, "");                 \
  static_assert(std::is_pod<T>{}, "");                             \
  static_assert(std::is_literal_type<T>{}, "");                    \
  static_assert(std::is_trivially_constructible<T>{}, "");         \
  static_assert(std::is_trivially_default_constructible<T>{}, ""); \
  static_assert(std::is_trivially_copy_constructible<T>{}, "");    \
  static_assert(std::is_trivially_move_constructible<T>{}, "");    \
  static_assert(std::is_trivially_assignable<T, T>{}, "");         \
  static_assert(std::is_trivially_copy_assignable<T>{}, "");       \
  static_assert(std::is_trivially_move_assignable<T>{}, "");       \
  static_assert(std::is_trivially_destructible<T>{}, "")
