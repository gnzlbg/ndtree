# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup project
##############################################################################
project(htree CXX)
cmake_minimum_required(VERSION 3.0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
enable_testing()

##############################################################################
# Setup CMake options
##############################################################################
include(CMakeDependentOption)
option(HTREE_ENABLE_VALGRIND "Run the unit tests and examples under Valgrind if it is found." OFF)
option(HTREE_ENABLE_ASAN "Run the unit tests and examples using AddressSanitizer." OFF)
option(HTREE_ENABLE_COVERAGE "Run the unit tests and examples with code coverage instrumentation." OFF)
option(HTREE_ENABLE_WERROR "Fail and stop if a warning is triggered." OFF)
option(HTREE_ENABLE_DEBUG_INFORMATION "Includes debug information in the binaries." OFF)
option(HTREE_ENABLE_ASSERTIONS "Enables assertions." OFF)

##############################################################################
# Setup compiler flags (more can be set on a per-target basis or in subdirectories)
##############################################################################
# Compiler flags:
include(CheckCXXCompilerFlag)
macro(htree_append_flag testname flag)
    check_cxx_compiler_flag(${flag} ${testname})
    if (${testname})
        add_compile_options(${flag})
    endif()
endmacro()

# Language flag: version of the C++ standard to use
htree_append_flag(HTREE_HAS_STDCXX14 -std=c++14)

# PITA warning flags:
htree_append_flag(HTREE_HAS_WSHADOW -Wshadow)
htree_append_flag(HTREE_HAS_WUNUSED -Wunused)
htree_append_flag(HTREE_HAS_WUNUSED_FUNCTION -Wunused-function)
htree_append_flag(HTREE_HAS_WUNUSED_LABEL -Wunused-label)
htree_append_flag(HTREE_HAS_WUNUSED_PARAMETER -Wunused-parameter)
htree_append_flag(HTREE_HAS_WUNUSED_VALUE -Wunused-value)
htree_append_flag(HTREE_HAS_WUNUSED_VARIABLE -Wunused-variable)

# Warning flags:
htree_append_flag(HTREE_HAS_WALL -Wall)
htree_append_flag(HTREE_HAS_WEXTRA -Wextra)
htree_append_flag(HTREE_HAS_WDEPRECATED -Wdeprecated)
htree_append_flag(HTREE_HAS_WDOCUMENTATION -Wdocumentation)
htree_append_flag(HTREE_HAS_WCOMMENT -Wcomment)
htree_append_flag(HTREE_HAS_PEDANTIC -Wpedantic)
htree_append_flag(HTREE_HAS_WSTACK_PROTECTOR -Wstack-protector)
htree_append_flag(HTREE_HAS_WSTRICT_ALIASING "-Wstrict-aliasing=2")
htree_append_flag(HTREE_HAS_WSTRICT_OVERFLOW "-Wstrict-overflow=5")
htree_append_flag(HTREE_HAS_WDISABLED_OPTIMIZATION -Wdisabled-optimization)
htree_append_flag(HTREE_HAS_WINLINE -Winline)
htree_append_flag(HTREE_HAS_WRETURN_TYPE -Wreturn-type)
htree_append_flag(HTREE_HAS_WCAST_ALIGN -Wcast-align)
htree_append_flag(HTREE_HAS_WCAST_QUAL -Wcast-qual)
htree_append_flag(HTREE_HAS_WSIGN_COMPARE -Wsign-compare)
htree_append_flag(HTREE_HAS_WSIGN_PROMO -Wsign-promo)
htree_append_flag(HTREE_HAS_WFORMAT "-Wformat=2")
htree_append_flag(HTREE_HAS_WFORMAT_NONLITERAL -Wformat-nonliteral)
htree_append_flag(HTREE_HAS_WFORMAT_SECURITY -Wformat-security)
htree_append_flag(HTREE_HAS_WFORMAT_Y2K -Wformat-y2k)
htree_append_flag(HTREE_HAS_WMISSING_BRACES -Wmissing-braces)
htree_append_flag(HTREE_HAS_WMISSING_FIELD_INITIALIZERS -Wmissing-field-initializers)
#htree_append_flag(HTREE_HAS_WMISSING_INCLUDE_DIRS -Wmissing-include-dirs)
htree_append_flag(HTREE_HAS_WOVERLOADED_VIRTUAL -Woverloaded-virtual)
htree_append_flag(HTREE_HAS_WCHAR_SUBSCRIPTS -Wchar-subscripts)
htree_append_flag(HTREE_HAS_WFLOAT_EQUAL -Wfloat-equal)
htree_append_flag(HTREE_HAS_WPOINTER_ARITH -Wpointer-arith)
htree_append_flag(HTREE_HAS_WINVALID_PCH -Winvalid-pch)
htree_append_flag(HTREE_HAS_WIMPORT -Wimport)
htree_append_flag(HTREE_HAS_WINIT_SELF -Winit-self)
htree_append_flag(HTREE_HAS_WREDUNDANT_DECLS -Wredundant-decls)
htree_append_flag(HTREE_HAS_WPACKED -Wpacked)
htree_append_flag(HTREE_HAS_WPARENTHESES -Wparentheses)
htree_append_flag(HTREE_HAS_WSEQUENCE_POINT -Wsequence-point)
htree_append_flag(HTREE_HAS_WSWITCH -Wswitch)
htree_append_flag(HTREE_HAS_WSWITCH_DEFAULT -Wswitch-default)
htree_append_flag(HTREE_HAS_WTRIGRAPHS -Wtrigraphs)
htree_append_flag(HTREE_HAS_WUNINITIALIZED -Wuninitialized)
htree_append_flag(HTREE_HAS_WUNKNOWN_PRAGMAS -Wunknown-pragmas)
htree_append_flag(HTREE_HAS_WUNREACHABLE_CODE -Wunreachable-code)
htree_append_flag(HTREE_HAS_WVARIADIC_MACROS -Wvariadic-macros)
htree_append_flag(HTREE_HAS_WVOLATILE_REGISTER_VAR -Wvolatile-register-var)
htree_append_flag(HTREE_HAS_WWRITE_STRINGS -Wwrite-strings)
htree_append_flag(HTREE_HAS_WNO_ATTRIBUTES -Wno-attributes)
htree_append_flag(HTREE_HAS_WUNNEEDED_INTERNAL_DECLARATION -Wunneeded-internal-declaration)

# Template diagnostic flags
htree_append_flag(HTREE_HAS_FDIAGNOSTIC_SHOW_TEMPLATE_TREE -fdiagnostics-show-template-tree)
htree_append_flag(HTREE_HAS_FTEMPLATE_BACKTRACE_LIMIT "-ftemplate-backtrace-limit=0")
htree_append_flag(HTREE_HAS___EXTERN_ALWAYS_INLINE -D__extern_always_inline=inline)

# Compiler flags controlled by CMake options above
if (HTREE_ENABLE_WERROR)
    htree_append_flag(HTREE_HAS_WERROR -Werror)
    htree_append_flag(HTREE_HAS_WX -WX)
  endif()

if (HTREE_ENABLE_ASAN)
  htree_append_flag(HTREE_HAS_SANITIZE "-fsanitize=address,integer,undefined,leak")
  htree_append_flag(HTREE_HAS_NO_OMIT_FRAME_POINTER -fno-omit-frame-pointer)
  htree_append_flag(HTREE_HAS_NO_SANITIZE "-fno-sanitize=unsigned-integer-overflow")
else()
  htree_append_flag(HTREE_HAS_OMIT_FRAME_POINTER -fomit-frame-pointer)
endif()

if (HTREE_ENABLE_DEBUG_INFORMATION)
  htree_append_flag(HTREE_HAS_G3 -g3)
else()
  htree_append_flag(HTREE_HAS_G0 -g0)
endif()

if (NOT HTREE_ENABLE_ASSERTIONS)
  htree_append_flag(HTREE_HAS_DHTREE_DISABLE_ASSERTIONS -DHTREE_DISABLE_ASSERTIONS)
endif()

if (HTREE_ENABLE_COVERAGE)
  if (CMAKE_BUILD_TYPE STREQUAL "Release")
    message(FATAL_ERROR "code coverage instrumentation requires CMAKE_BUILD_TYPE=Debug")
  endif()
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage")
endif()

# Optimization flags: debug
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
  htree_append_flag(HTREE_HAS_O0 -O0)
  htree_append_flag(HTREE_HAS_NO_INLINE -fno-inline)
  htree_append_flag(HTREE_HAS_STACK_PROTECTOR_ALL -fstack-protector-all)
endif()

# Optimization flags: release
if (CMAKE_BUILD_TYPE STREQUAL "Release")
  htree_append_flag(HTREE_HAS_OFAST -Ofast)
  htree_append_flag(HTREE_HAS_DNDEBUG -DNDEBUG)
  htree_append_flag(HTREE_HAS_MARCH_NATIVE "-march=native")
  htree_append_flag(HTREE_HAS_MTUNE_NATIVE "-mtune=native")
  htree_append_flag(HTREE_HAS_STRICT_ALIASING -fstrict-aliasing)
  htree_append_flag(HTREE_HAS_VECTORIZE -fvectorize)
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

macro(htree_pkg name cflags lflags)
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
    OUTPUT_VARIABLE HTREE_CURRENT_COMMIT
    OUTPUT_STRIP_TRAILING_WHITESPACE
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# Doxygen
find_package(Doxygen)

# Valgrid
if (HTREE_ENABLE_VALGRIND)
  find_program(MEMORYCHECK_COMMAND valgrind)
  if(MEMORYCHECK_COMMAND-NOTFOUND)
    message(FATAL_ERROR "valgrind not found")
  endif()
  set(MEMORYCHECK_COMMAND_OPTIONS "--trace-children=yes --leak-check=full")
  include(Dart)
endif()

# Packages
htree_pkg(cppformat "-DFMT_HEADER_ONLY" "")
htree_pkg(range-v3 "-DRANGES_SUPPRESS_IOTA_WARNING" "")

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
  set(HTREE_ORIGIN_URL "https://gnzlbg:$ENV{GITHUB_TOKEN}@github.com/gnzlbg/htree")
else()
  set(HTREE_ORIGIN_URL "https://github.com/gnzlbg/htree")
endif()


##############################################################################
# Setup custom functions to ease the creation of targets
##############################################################################
#   htree_target_name_for(<output variable> <source file> [ext])
#
# Return the target name associated to a source file. If the path of the
# source file relative from the root of htree is `path/to/source/file.ext`,
# the target name associated to it will be `path.to.source.file`.
#
# The extension of the file should be specified as a last argument. If no
# extension is specified, the `.cpp` extension is assumed.
function(htree_target_name_for out file)
    if (NOT ARGV2)
        set(_extension ".cpp")
    else()
        set(_extension "${ARGV2}")
    endif()

    file(RELATIVE_PATH _relative ${htree_SOURCE_DIR} ${file})
    string(REPLACE "${_extension}" "" _name ${_relative})
    string(REGEX REPLACE "/" "." _name ${_name})
    set(${out} "${_name}" PARENT_SCOPE)
endfunction()

#   htree_add_test(<name> <command> [<arg>...])
#
# Creates a test called `name`, which runs the given `command` with the given
# `arg`uments.
function(htree_add_test name)
  add_test(${name} ${ARGN})
  add_dependencies(${name} fetch_packages)
endfunction()

#   htree_list_remove_glob(<list> <GLOB|GLOB_RECURSE> [globbing expressions]...)
#
# Generates a list of files matching the given glob expressions, and remove
# the matched elements from the given <list>.
macro(htree_list_remove_glob list glob)
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
    COMMAND ${CMAKE_CTEST_COMMAND} --VV #--output-on-failure
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS tests examples
    COMMENT "Build and then run all the tests and examples.")

