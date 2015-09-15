# Copyright Gonzalo Brito Gadeschi 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
# Find the cppformat include directory
# The following variables are set if cppformat is found.
#  cppformat_FOUND        - True when the cppformat include directory is found.
#  cppformat_INCLUDE_DIR  - The path to where the cppformat include files are.
# If cppformat is not found, cppformat_FOUND is set to false.

find_package(PkgConfig)

if(NOT EXISTS "${cppformat_INCLUDE_DIR}")
  find_path(cppformat_INCLUDE_DIR
    NAMES cppformat/cppformat.h
    DOC "cppformat library header files"
    )
endif()

if(EXISTS "${cppformat_INCLUDE_DIR}")
  include(FindPackageHandleStandardArgs)
  mark_as_advanced(cppformat_INCLUDE_DIR)
else()
  include(ExternalProject)
  ExternalProject_Add(cppformat
    GIT_REPOSITORY https://github.com/cppformat/cppformat.git
    TIMEOUT 5
    CMAKE_ARGS -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} -DCMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS}
    PREFIX "${CMAKE_CURRENT_BINARY_DIR}"
    CONFIGURE_COMMAND "" # Disable configure step
    BUILD_COMMAND "" # Disable build step
    INSTALL_COMMAND "" # Disable install step
    UPDATE_COMMAND "" # Disable update step: clones the project only once
    )
  
  # Specify include dir
  ExternalProject_Get_Property(cppformat source_dir)
  set(cppformat_INCLUDE_DIR ${source_dir})
endif()

if(EXISTS "${cppformat_INCLUDE_DIR}")
  set(cppformat_FOUND 1)
else()
  set(cppformat_FOUND 0)
endif()
