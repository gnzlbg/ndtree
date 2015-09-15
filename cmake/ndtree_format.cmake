# Copyright Gonzalo Brito Gadeschi 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)
#
##############################################################################
# Set up the `format` target to re-format/check all the files in:
# - include/
# - test/
# - example/
# using clang-format.
#
# Two targets are added:
# - make format: reformats all files
# - make check-format: check the format of all files
##############################################################################

find_program(CLANG_FORMAT NAMES clang-format clang-format-3.8 clang-format-3.7 clang-format-3.6)
if(CLANG_FORMAT)
  add_custom_command(OUTPUT format-cmd COMMAND
    ${PROJECT_SOURCE_DIR}/cmake/format.py ${CLANG_FORMAT} ${PROJECT_SOURCE_DIR} --apply)
  add_custom_target(format DEPENDS format-cmd WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
  
  add_custom_command(OUTPUT check-format-cmd COMMAND
    ${PROJECT_SOURCE_DIR}/cmake/format.py ${CLANG_FORMAT} ${PROJECT_SOURCE_DIR})
  add_custom_target(check-format DEPENDS check-format-cmd WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
endif()
