# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup project
##############################################################################
project(ndtree CXX)
cmake_minimum_required(VERSION 3.0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
enable_testing()

##############################################################################
# Setup CMake options
##############################################################################
include(CMakeDependentOption)
option(NDTREE_ENABLE_VALGRIND "Run the unit tests and examples under Valgrind if it is found." OFF)
option(NDTREE_ENABLE_ASAN "Run the unit tests and examples using AddressSanitizer." OFF)
option(NDTREE_ENABLE_COVERAGE "Run the unit tests and examples with code coverage instrumentation." OFF)
option(NDTREE_ENABLE_WERROR "Fail and stop if a warning is triggered." OFF)
option(NDTREE_ENABLE_DEBUG_INFORMATION "Includes debug information in the binaries." OFF)
option(NDTREE_ENABLE_ASSERTIONS "Enables assertions." OFF)

##############################################################################
# Setup compiler flags (more can be set on a per-target basis or in subdirectories)
##############################################################################
# Compiler flags:
include(CheckCXXCompilerFlag)
macro(ndtree_append_flag testname flag)
    check_cxx_compiler_flag(${flag} ${testname})
    if (${testname})
        add_compile_options(${flag})
    endif()
endmacro()

# Language flag: version of the C++ standard to use
ndtree_append_flag(NDTREE_HAS_STDCXX14 -std=c++14)

# PITA warning flags:
ndtree_append_flag(NDTREE_HAS_WSHADOW -Wshadow)
ndtree_append_flag(NDTREE_HAS_WUNUSED -Wunused)
ndtree_append_flag(NDTREE_HAS_WUNUSED_FUNCTION -Wunused-function)
ndtree_append_flag(NDTREE_HAS_WUNUSED_LABEL -Wunused-label)
ndtree_append_flag(NDTREE_HAS_WUNUSED_PARAMETER -Wunused-parameter)
ndtree_append_flag(NDTREE_HAS_WUNUSED_VALUE -Wunused-value)
ndtree_append_flag(NDTREE_HAS_WUNUSED_VARIABLE -Wunused-variable)

# Warning flags:
ndtree_append_flag(NDTREE_HAS_WALL -Wall)
ndtree_append_flag(NDTREE_HAS_WEXTRA -Wextra)
ndtree_append_flag(NDTREE_HAS_WDEPRECATED -Wdeprecated)
ndtree_append_flag(NDTREE_HAS_WDOCUMENTATION -Wdocumentation)
ndtree_append_flag(NDTREE_HAS_WCOMMENT -Wcomment)
ndtree_append_flag(NDTREE_HAS_PEDANTIC -Wpedantic)
ndtree_append_flag(NDTREE_HAS_WSTACK_PROTECTOR -Wstack-protector)
ndtree_append_flag(NDTREE_HAS_WSTRICT_ALIASING "-Wstrict-aliasing=2")
ndtree_append_flag(NDTREE_HAS_WSTRICT_OVERFLOW "-Wstrict-overflow=5")
ndtree_append_flag(NDTREE_HAS_WDISABLED_OPTIMIZATION -Wdisabled-optimization)
ndtree_append_flag(NDTREE_HAS_WINLINE -Winline)
ndtree_append_flag(NDTREE_HAS_WRETURN_TYPE -Wreturn-type)
ndtree_append_flag(NDTREE_HAS_WCAST_ALIGN -Wcast-align)
ndtree_append_flag(NDTREE_HAS_WCAST_QUAL -Wcast-qual)
ndtree_append_flag(NDTREE_HAS_WSIGN_COMPARE -Wsign-compare)
ndtree_append_flag(NDTREE_HAS_WSIGN_PROMO -Wsign-promo)
ndtree_append_flag(NDTREE_HAS_WFORMAT "-Wformat=2")
ndtree_append_flag(NDTREE_HAS_WFORMAT_NONLITERAL -Wformat-nonliteral)
ndtree_append_flag(NDTREE_HAS_WFORMAT_SECURITY -Wformat-security)
ndtree_append_flag(NDTREE_HAS_WFORMAT_Y2K -Wformat-y2k)
ndtree_append_flag(NDTREE_HAS_WMISSING_BRACES -Wmissing-braces)
ndtree_append_flag(NDTREE_HAS_WMISSING_FIELD_INITIALIZERS -Wmissing-field-initializers)
#ndtree_append_flag(NDTREE_HAS_WMISSING_INCLUDE_DIRS -Wmissing-include-dirs)
ndtree_append_flag(NDTREE_HAS_WOVERLOADED_VIRTUAL -Woverloaded-virtual)
ndtree_append_flag(NDTREE_HAS_WCHAR_SUBSCRIPTS -Wchar-subscripts)
ndtree_append_flag(NDTREE_HAS_WFLOAT_EQUAL -Wfloat-equal)
ndtree_append_flag(NDTREE_HAS_WPOINTER_ARITH -Wpointer-arith)
ndtree_append_flag(NDTREE_HAS_WINVALID_PCH -Winvalid-pch)
ndtree_append_flag(NDTREE_HAS_WIMPORT -Wimport)
ndtree_append_flag(NDTREE_HAS_WINIT_SELF -Winit-self)
ndtree_append_flag(NDTREE_HAS_WREDUNDANT_DECLS -Wredundant-decls)
ndtree_append_flag(NDTREE_HAS_WPACKED -Wpacked)
ndtree_append_flag(NDTREE_HAS_WPARENTHESES -Wparentheses)
ndtree_append_flag(NDTREE_HAS_WSEQUENCE_POINT -Wsequence-point)
ndtree_append_flag(NDTREE_HAS_WSWITCH -Wswitch)
ndtree_append_flag(NDTREE_HAS_WSWITCH_DEFAULT -Wswitch-default)
ndtree_append_flag(NDTREE_HAS_WTRIGRAPHS -Wtrigraphs)
ndtree_append_flag(NDTREE_HAS_WUNINITIALIZED -Wuninitialized)
ndtree_append_flag(NDTREE_HAS_WUNKNOWN_PRAGMAS -Wunknown-pragmas)
ndtree_append_flag(NDTREE_HAS_WUNREACHABLE_CODE -Wunreachable-code)
ndtree_append_flag(NDTREE_HAS_WVARIADIC_MACROS -Wvariadic-macros)
ndtree_append_flag(NDTREE_HAS_WVOLATILE_REGISTER_VAR -Wvolatile-register-var)
ndtree_append_flag(NDTREE_HAS_WWRITE_STRINGS -Wwrite-strings)
ndtree_append_flag(NDTREE_HAS_WNO_ATTRIBUTES -Wno-attributes)
ndtree_append_flag(NDTREE_HAS_WUNNEEDED_INTERNAL_DECLARATION -Wunneeded-internal-declaration)

# Template diagnostic flags
ndtree_append_flag(NDTREE_HAS_FDIAGNOSTIC_SHOW_TEMPLATE_TREE -fdiagnostics-show-template-tree)
ndtree_append_flag(NDTREE_HAS_FTEMPLATE_BACKTRACE_LIMIT "-ftemplate-backtrace-limit=0")
ndtree_append_flag(NDTREE_HAS___EXTERN_ALWAYS_INLINE -D__extern_always_inline=inline)

# Compiler flags controlled by CMake options above
if (NDTREE_ENABLE_WERROR)
  ndtree_append_flag(NDTREE_HAS_WERROR -Werror)
  ndtree_append_flag(NDTREE_HAS_WX -WX)
endif()

if (NDTREE_ENABLE_ASAN)
  set(NDTREE_ASAN_FLAGS "-fsanitize=address,integer,undefined,leak")
  # Currently failing:
  # ndtree_append_flag(NDTREE_HAS_SANITIZE "${NDTREE_ASAN_FLAGS}")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${NDTREE_ASAN_FLAGS}")
  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${NDTREE_ASAN_FLAGS}")
  ndtree_append_flag(NDTREE_HAS_NO_OMIT_FRAME_POINTER -fno-omit-frame-pointer)
  ndtree_append_flag(NDTREE_HAS_NO_SANITIZE "-fno-sanitize=unsigned-integer-overflow")
else()
  ndtree_append_flag(NDTREE_HAS_OMIT_FRAME_POINTER -fomit-frame-pointer)
endif()

if (NDTREE_ENABLE_DEBUG_INFORMATION)
  ndtree_append_flag(NDTREE_HAS_G3 -g3)
else()
  ndtree_append_flag(NDTREE_HAS_G0 -g0)
endif()

if (NOT NDTREE_ENABLE_ASSERTIONS)
  ndtree_append_flag(NDTREE_HAS_DNDTREE_DISABLE_ASSERTIONS -DNDTREE_DISABLE_ASSERTIONS)
endif()

if (NDTREE_ENABLE_COVERAGE)
  if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(FATAL_ERROR "code coverage instrumentation requires CMAKE_BUILD_TYPE=Debug")
  endif()
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

# Optimization flags: debug
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  ndtree_append_flag(NDTREE_HAS_O0 -O0)
  ndtree_append_flag(NDTREE_HAS_NO_INLINE -fno-inline)
  ndtree_append_flag(NDTREE_HAS_STACK_PROTECTOR_ALL -fstack-protector-all)
endif()

# Optimization flags: release
if (CMAKE_BUILD_TYPE STREQUAL "Release")
  ndtree_append_flag(NDTREE_HAS_OFAST -Ofast)
  ndtree_append_flag(NDTREE_HAS_DNDEBUG -DNDEBUG)
  ndtree_append_flag(NDTREE_HAS_MARCH_NATIVE "-march=native")
  ndtree_append_flag(NDTREE_HAS_MTUNE_NATIVE "-mtune=native")
  ndtree_append_flag(NDTREE_HAS_STRICT_ALIASING -fstrict-aliasing)
  ndtree_append_flag(NDTREE_HAS_VECTORIZE -fvectorize)
  # Others could be: -pipe pfdata-sections -ffunction-sections
  # for clang: -mllvm -inline-threshold=X (X=1000, 10000, ...)
endif()

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

# Valgrid
if (NDTREE_ENABLE_VALGRIND)
  find_program(MEMORYCHECK_COMMAND valgrind)
  if(MEMORYCHECK_COMMAND-NOTFOUND)
    message(FATAL_ERROR "valgrind not found")
  endif()
  set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")
  include(Dart)
endif()

# Packages
ndtree_pkg(cppformat "-DFMT_HEADER_ONLY" "")
ndtree_pkg(range-v3 "-DRANGES_SUPPRESS_IOTA_WARNING" "")

##############################################################################
# Setup the URL to the official repository on GitHub.
#
# If there is a GITHUB_TOKEN environment variable, we assume this token allows
# us to push to the GitHub repository and we use that in the URL. This token
# will give us push access for updating the 'gh-pages' branch and doing other
# useful things e.g. from Travis. Otherwise, if there's no such environment
# variable, the URL just refers to the GitHub repository without authentication.
# In this case, we'll be asked for our username and password when/if we try to
# push to the repository.
##############################################################################
if (DEFINED ENV{GITHUB_TOKEN})
  set(NDTREE_ORIGIN_URL "https://gnzlbg:$ENV{GITHUB_TOKEN}@github.com/gnzlbg/ndtree")
else()
  set(NDTREE_ORIGIN_URL "https://github.com/gnzlbg/ndtree")
endif()


##############################################################################
# Setup custom functions to ease the creation of targets
##############################################################################
#   ndtree_target_name_for(<output variable> <source file> [ext])
#
# Return the target name associated to a source file. If the path of the
# source file relative from the root of ndtree is `path/to/source/file.ext`,
# the target name associated to it will be `path.to.source.file`.
#
# The extension of the file should be specified as a last argument. If no
# extension is specified, the `.cpp` extension is assumed.
function(ndtree_target_name_for out file)
    if (NOT ARGV2)
        set(_extension ".cpp")
    else()
        set(_extension "${ARGV2}")
    endif()

    file(RELATIVE_PATH _relative ${ndtree_SOURCE_DIR} ${file})
    string(REPLACE "${_extension}" "" _name ${_relative})
    string(REGEX REPLACE "/" "." _name ${_name})
    set(${out} "${_name}" PARENT_SCOPE)
endfunction()

#   ndtree_add_test(<name> <command> [<arg>...])
#
# Creates a test called `name`, which runs the given `command` with the given
# `arg`uments.
function(ndtree_add_test name)
  add_test(${name} ${ARGN})
  add_dependencies(${name} fetch_packages)
endfunction()

#   ndtree_list_remove_glob(<list> <GLOB|GLOB_RECURSE> [globbing expressions]...)
#
# Generates a list of files matching the given glob expressions, and remove
# the matched elements from the given <list>.
macro(ndtree_list_remove_glob list glob)
    file(${glob} _bhlrg10321023_avoid_macro_clash_matches ${ARGN})
    list(REMOVE_ITEM ${list} ${_bhlrg10321023_avoid_macro_clash_matches})
endmacro()


##############################################################################
# Setup subdirectories
##############################################################################
#add_subdirectory(benchmark)
add_subdirectory(site)
add_subdirectory(example)
add_subdirectory(test)


##############################################################################
# Setup the `check` target to build and then run all the tests and examples.
##############################################################################
add_custom_target(check ALL
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure -R "test.+"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS tests examples
  COMMENT "Build and then run all the tests and examples.")

