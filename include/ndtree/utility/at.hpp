#pragma once
/// \file at.hpp NDTREE_AT macro
#include <ndtree/utility/fmt.hpp>

namespace ndtree {
inline namespace v1 {
struct source_location {
  template <class A, class B, class C>
  [[always_inline]] constexpr source_location(A&& func, B&& file, C&& line)
   : function_name(func), file_name(file), line_number(line) {}

  const char* const function_name;
  const char* const file_name;
  const int line_number;
};

}  // namespace v1
}  // namespace ndtree

/// Returns a source code locator containing the position where it is
/// defined.
#define NDTREE_AT_ \
  ::ndtree::source_location(__PRETTY_FUNCTION__, __FILE__, __LINE__)

/// Formats a source code location
#define NDTREE_FMT_AT(AT)                                                      \
  ::ndtree::fmt::format("  at function: {}\n  at file: {}\n  at line: {}\n\n", \
                        (AT).function_name, (AT).file_name, (AT).line_number)

/// Formats a source code location
#define NDTREE_PRINT_AT(AT) ::ndtree::fmt::print(NDTREE_FMT_AT(AT))
