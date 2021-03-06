/* This is free and unencumbered software released into the public domain. */

#ifndef BITCACHE_ARCH_H
#define BITCACHE_ARCH_H

#ifdef __cplusplus
extern "C" {
#endif

#ifndef GCC_VERSION
# define GCC_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
#endif

/* GCC-specific optimizations */
// @see http://gcc.gnu.org/onlinedocs/gcc/Function-Attributes.html
#ifdef __GNUC__
# define NONNULL __attribute__((__nonnull__)) /* the function requires non-NULL arguments */
# define FLATTEN __attribute__((__flatten__)  /* inline every call inside the function, if possible */
# define PURE    __attribute__((__pure__))    /* declare that the function has no side effects */
# if GCC_VERSION >= 4003
#  define HOT    __attribute__((__hot__))     /* the function is a hot spot (GCC 4.3+ only) */
#  define COLD   __attribute__((__cold__))    /* the function is unlikely to be executed (GCC 4.3+ only) */
# else
#  define HOT
#  define COLD
# endif
#else  /* !__GNUC__ */
# define NONNULL
# define FLATTEN
# define PURE
# define HOT
# define COLD
#endif /* __GNUC__ */

/* branch prediction hints */
#ifdef __GNUC__
# define likely(x)         __builtin_expect(!!(x), 1) /* `x` is likely to evaluate to TRUE   */
# define unlikely(x)       __builtin_expect(!!(x), 0) /* `x` is unlikely to evaluate to TRUE */
#else
# define likely(x)         x
# define unlikely(x)       x
#endif /* __GNUC__ */

#ifdef __cplusplus
}
#endif

#endif /* BITCACHE_ARCH_H */
