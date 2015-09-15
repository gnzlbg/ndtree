# Copyright Gonzalo Brito Gadeschi 2015
# Copyright Louis Dionne 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup project
##############################################################################
project(ndtree CXX)
cmake_minimum_required(VERSION 3.0)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
enable_testing()

include(ndtree_options)
include(ndtree_compiler_flags)
include(ndtree_packages)
include(ndtree_targets)
include(ndtree_format)

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
# Setup subdirectoriescmake
##############################################################################
add_subdirectory(test)
add_subdirectory(example)
add_subdirectory(site)

##############################################################################
# Setup the `check` target to build, check format, and then run all the tests
# and examples.
##############################################################################
add_custom_target(check ALL
  COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  DEPENDS check-format tests test.headers examples
  COMMENT "Build and then run all the tests and examples.")
