#pragma once
/// \file terminate.hpp Program termination routines
#include <stdexcept>
#include <ndtree/utility/fmt.hpp>
#include <ndtree/utility/at.hpp>

// This requires the gnu/clang zero-variadic-macro-arguments language extension
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

/// Terminates the program
///
/// TODO: offer a way to throw an exception with the message instead
#define NDTREE_TERMINATE(message, ...)                                       \
  [[ cold, noinline ]] do {                                                  \
    ::ndtree::fmt::print(                                                    \
     stderr, "\nFATAL ERROR:\n\n  message: " message "\n\n", ##__VA_ARGS__); \
    NDTREE_PRINT_AT(NDTREE_AT_);                                             \
    std::terminate();                                                        \
  }                                                                          \
  while (false)

#pragma clang diagnostic pop
