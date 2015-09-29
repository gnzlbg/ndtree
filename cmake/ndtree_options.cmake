# Copyright Gonzalo Brito Gadeschi 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
# CMake options

include(CMakeDependentOption)

option(NDTREE_ENABLE_ASAN "Run the unit tests and examples using AddressSanitizer." OFF)
option(NDTREE_ENABLE_COVERAGE "Run the unit tests and examples with code coverage instrumentation." OFF)
option(NDTREE_ENABLE_WERROR "Fail and stop if a warning is triggered." OFF)
option(NDTREE_ENABLE_DEBUG_INFORMATION "Includes debug information in the binaries." OFF)
option(NDTREE_ENABLE_ASSERTIONS "Enables assertions." OFF)
