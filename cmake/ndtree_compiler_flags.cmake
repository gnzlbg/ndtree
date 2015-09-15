# Copyright Louis Dionne 2015
# Copyright Gonzalo Brito Gadeschi 2015
# Distributed under the Boost Software License, Version 1.0.
# (See accompanying file LICENSE.md or copy at http://boost.org/LICENSE_1_0.txt)

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
  #set(NDTREE_ASAN_FLAGS "-fsanitize=address,integer,undefined,leak -fno-omit-frame-pointer -fno-sanitize=unsigned-integer-overflow")
  set(NDTREE_ASAN_FLAGS "-fsanitize=address,integer,undefined,leak -fno-omit-frame-pointer")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${NDTREE_ASAN_FLANGS}")
  ndtree_append_flag(NDTREE_HAS_SANITIZE "${NDTREE_ASAN_FLAGS}")
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
