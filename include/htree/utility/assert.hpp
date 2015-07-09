#pragma once
/// \file assert.hpp Assert macro
#include <htree/utility/at.hpp>
#include <htree/utility/likely.hpp>
#include <htree/utility/fmt.hpp>

// This requires the gnu/clang zero-variadic-macro-arguments language extension
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

/// Asserts a condition in DEBUG mode. Useful for checking
/// pre/postconditions and invariants.
///
/// \warning if HTREE_DISABLE_ASSERTIONS is defined the condition in the if
/// clause
/// is always false and HTREE_ASSERT compiles to nothing! Still this happens
/// after
/// the compiler has verified the correctness of the HTREE_ASSERT code.
#ifndef HTREE_DISABLE_ASSERTIONS
#define HTREE_ASSERT_AT(condition, message, AT, ...)                       \
  do {                                                                     \
    if (HTREE_UNLIKELY(!(condition))) {                                    \
      ::htree::fmt::print(                                                 \
       stderr,                                                             \
       "\nASSERTION FAILED:\n\n  condition: \"{}\"\n  message: \"" message \
       "\"\n\n",                                                           \
       #condition, ##__VA_ARGS__);                                         \
      HTREE_PRINT_AT(AT);                                                  \
      __builtin_debugtrap();                                               \
    }                                                                      \
  } while (false)
#else  // HTREE_DISABLE_ASSERTIONS
#define HTREE_ASSERT_AT(condition, message, AT, ...)                       \
  do {                                                                     \
    if (false && HTREE_UNLIKELY(!(condition))) {                           \
      ::htree::fmt::print(                                                 \
       stderr,                                                             \
       "\nASSERTION FAILED:\n\n  condition: \"{}\"\n  message: \"" message \
       "\"\n",                                                             \
       #condition, ##__VA_ARGS__);                                         \
      HTREE_PRINT_AT(AT);                                                  \
      __builtin_debugtrap();                                               \
    }                                                                      \
  } while (false)
#endif  // HTREE_DISABLE_ASSERTIONS

#define HTREE_ASSERT(condition, message, ...) \
  HTREE_ASSERT_AT(condition, message, HTREE_AT_, ##__VA_ARGS__)

#pragma clang diagnostic pop

#define HTREE_ASSERT_STANDARD_LAYOUT(T)                            \
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
