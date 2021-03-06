# Copyright Louis Dionne 2015
# Copyright Gonzalo Brito Gadeschi 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

##############################################################################
# Setup custom functions, master targets and file lists for the examples
##############################################################################
add_custom_target(examples
    COMMENT "Build all the examples and adds them as tests.")

# Several examples have unused parameters because the name of the parameters
# are useful for illustration, even if the implementation is not actually
# presented. We don't want to generate warnings for that or need to comment
# out all unused parameter names.
check_cxx_compiler_flag(-Wno-unused-parameter NDTREE_HAS_WNO_UNUSED_PARAMETER)

#   ndtree_add_example(<name> ...)
#
# Equivalent to `ndtree_add_test`, except the created test is also added
# as a dependency of the `examples` target.
function(ndtree_add_example name)
  ndtree_add_unit_test(${name} ${ARGN})
  add_dependencies(examples ${name})
endfunction()

# A list of all the example files
#
# Examples that are not available due to caveats are removed
# from this list below.
file(GLOB_RECURSE NDTREE_EXAMPLE_SOURCES "${ndtree_SOURCE_DIR}/example/*.cpp")

##############################################################################
# Add all the examples
##############################################################################
foreach(_file IN LISTS NDTREE_EXAMPLE_SOURCES)
    ndtree_target_name_for(_target "${_file}")
    add_executable(${_target} EXCLUDE_FROM_ALL "${_file}")
    ndtree_add_example(${_target} ${CMAKE_CURRENT_BINARY_DIR}/${_target})
    if(NDTREE_HAS_WNO_UNUSED_PARAMETER)
      set_target_properties(${_target} PROPERTIES COMPILE_FLAGS "-Wno-unused-parameter")
    endif()
endforeach()
