#pragma once
/// \file likely.hpp LIKELY/UNLIKELY Macros (hints for branch prediction)
///
/// Use like this:
///
/// if(NDTREE_UNLIKELY(1 == 0)) { ... }
/// int f(bool);
/// bool b = true;
/// int a = f(NDTREE_LIKELY(b));
///
/// To disable LIKELY/UNLIKELY: define NDTREE_DISABLE_LIKELY_MACROS

#ifndef NDTREE_DISABLE_LIKELY_MACROS
#define NDTREE_LIKELY(boolean_expr) __builtin_expect((boolean_expr), 1)
#define NDTREE_UNLIKELY(boolean_expr) __builtin_expect((boolean_expr), 0)
#else
#define NDTREE_LIKELY(boolean_expr) (boolean_expr)
#define NDTREE_UNLIKELY(boolean_expr) (boolean_expr)
#endif
