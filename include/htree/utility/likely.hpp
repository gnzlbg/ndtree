#pragma once
/// \file LIKELY/UNLIKELY Macros (hints for branch prediction)
///
/// Use like this:
///
/// if(HTREE_UNLIKELY(1 == 0)) { ... }
/// int f(bool);
/// bool b = true;
/// int a = f(HTREE_LIKELY(b));
///
/// To disable LIKELY/UNLIKELY: define HTREE_DISABLE_LIKELY_MACROS

#ifndef HTREE_DISABLE_LIKELY_MACROS
#define HTREE_LIKELY(boolean_expr) __builtin_expect((boolean_expr), 1)
#define HTREE_UNLIKELY(boolean_expr) __builtin_expect((boolean_expr), 0)
#else
#define HTREE_LIKELY(boolean_expr) (boolean_expr)
#define HTREE_UNLIKELY(boolean_expr) (boolean_expr)
#endif
