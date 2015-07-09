#pragma once
/// \file at.hpp HTREE_AT macro
#include <htree/utility/fmt.hpp>

namespace htree {

struct source_location {
  template <class A, class B, class C>
  [[always_inline]] constexpr source_location(A&& func, B&& file, C&& line)
   : function_name(func), file_name(file), line_number(line) {}

  const char* const function_name;
  const char* const file_name;
  const int line_number;
};

/// Returns a source code locator containing the position where it is
/// defined.
#define HTREE_AT_ \
  ::htree::source_location(__PRETTY_FUNCTION__, __FILE__, __LINE__)

}  // namespace htree

/// Formats a source code location
#define HTREE_FMT_AT(AT)                                             \
  ::htree::fmt::format("  at function: {}\n  at file: {}\n  at line: {}\n\n", \
              AT.function_name, AT.file_name, AT.line_number)

/// Formats a source code location
#define HTREE_PRINT_AT(AT) ::htree::fmt::print(HTREE_FMT_AT(AT))
