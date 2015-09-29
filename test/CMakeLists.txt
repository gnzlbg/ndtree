# Copyright Louis Dionne 2015
# Copyright Gonzalo Brito Gadeschi 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

# Setup custom functions, master targets and file lists for the unit tests
add_custom_target(test.headers
  COMMENT "Build all the header-inclusion unit tests.")

add_custom_target(tests
  DEPENDS test.headers
  COMMENT "Build all the unit tests.")

#   ndtree_add_unit_test(<name> ...)
#
# Equivalent to `ndtree_add_test`, except the test is also added as a
# dependency of the `tests` target.
function(ndtree_add_unit_test name)
  add_test(${name} ${ARGN})
  ndtree_add_packages_to_target(${name})
  add_dependencies(tests ${name})
endfunction()

#   ndtree_add_header_test(<header-name>)
#
# Add a unit test for the public header file `header-name`, which must be a
# relative path from ndtree's include directory, e.g. `ndtree/ndtree.hpp`.
#
# This function creates an executable named `header.header-name` and a test
# of the same name. The only source file of the executable contains an empty
# `main` and it includes the `header-name` file. This is used to make sure
# that including any public header works properly. Also, the
# `header.header-name` target is made a dependency of the `headers` target.
function(ndtree_add_header_test header)
  string(REGEX REPLACE "/" "." _target "${header}")
  if (NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp")
    file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp" "
            #include <${header}>
            int main() { return 0; }
        ")
  endif()
  add_executable(test.header.${_target} EXCLUDE_FROM_ALL
    "${CMAKE_CURRENT_BINARY_DIR}/header/${header}.cpp")
  ndtree_add_packages_to_target(test.header.${_target})
  add_dependencies(test.headers test.header.${_target})
endfunction()

# A list of all the test files
file(GLOB_RECURSE NDTREE_TEST_SOURCES "${ndtree_SOURCE_DIR}/test/*.cpp")

# A list of all the public headers
file(GLOB_RECURSE NDTREE_PUBLIC_HEADERS "${ndtree_SOURCE_DIR}/include/*.hpp")

# Generate tests that include each public header
foreach(_header IN LISTS NDTREE_PUBLIC_HEADERS)
  file(RELATIVE_PATH _relative "${ndtree_SOURCE_DIR}/include" "${_header}")
  ndtree_add_header_test("${_relative}")
endforeach()

# Add all the unit tests
foreach(_file IN LISTS NDTREE_TEST_SOURCES)
  file(READ "${_file}" _contents)
  ndtree_target_name_for(_target "${_file}")

  add_executable(${_target} EXCLUDE_FROM_ALL "${_file}")
  ndtree_add_unit_test(${_target} ${CMAKE_CURRENT_BINARY_DIR}/${_target})
endforeach()
