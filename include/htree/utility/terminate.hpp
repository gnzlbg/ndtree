#pragma once
/// \file terminate.hpp Program termination routines
#include <stdexcept>
#include <format.h>
#include <htree/utility/at.hpp>

// This requires the gnu/clang zero-variadic-macro-arguments language extension
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"

/// Terminates the program
#define HTREE_TERMINATE(message, ...)                                     \
  [[ cold, noinline ]] do {                                             \
    htree::fmt::print(stderr, "\nFATAL ERROR:\n\n  message: " message "\n\n", \
                    ##__VA_ARGS__);                                     \
    HTREE_PRINT_AT(HTREE_AT_);                                              \
    std::terminate();                                                   \
  }                                                                     \
  while (false)

#pragma clang diagnostic pop
