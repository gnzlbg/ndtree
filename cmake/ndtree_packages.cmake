# Copyright Gonzalo Brito Gadeschi 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Search for packages.
#
# Behavior when the package is found or not is customized at the
# point where the package is required.
##############################################################################

# Target for fetching packages
add_custom_target(fetch_packages)

macro(ndtree_pkg name cflags lflags)
  find_package(${name} REQUIRED)
  include_directories(${${name}_INCLUDE_DIR})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${${name}_COMPILE_FLAGS} ${cflags}")
  set(CMAKE_CXX_LINK_FLAGS "${CMAKE_CXX_LINK_FLAGS} ${${name}_LINK_FLAGS} ${lflags}")
  include_directories(SYSTEM ${${name}_INCLUDE_DIR})
  add_dependencies(fetch_packages ${name})
endmacro()


# Git: parses current project commit
find_package(Git)
if (GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
    OUTPUT_VARIABLE NDTREE_CURRENT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Doxygen
find_package(Doxygen)

# Valgrind
find_program(MEMORYCHECK_COMMAND valgrind)
if(MEMORYCHECK_COMMAND-NOTFOUND)
  message(FATAL_ERROR "valgrind not found")
else()
  set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")
  include(Dart)
endif()

# Packages
ndtree_pkg(cppformat "-DFMT_HEADER_ONLY" "")
ndtree_pkg(range-v3 "-DRANGES_SUPPRESS_IOTA_WARNING" "")
