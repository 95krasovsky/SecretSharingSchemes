
#ifndef NTL_ctools__H
#define NTL_ctools__H

#include "config.h"
#include "mach_desc.h"
#include "HAVE_LL_TYPE.h"
#include "HAVE_BUILTIN_CLZL.h"
#include "HAVE_AVX.h"
#include "HAVE_FMA.h"


/*
 * Resolve double-word integer types.
 *
 * Unfortunately, there is no "standard" way to do this.
 * On 32-bit machines, 'long long' usually works (but not
 * on MSVC++ or BORLAND), and on 64-bit machines, there is
 * no standard.  However, most compilers do offer *some*
 * non-standard double-word type.  
 *
 * Note that C99 creates a standard header <stdint.h>,
 * but it is not clear how widely this is implemented yet,
 * and for example, GCC does not provide a type int128_t 
 * in <stdint.h> on 64-bit machines.
 */


#if (defined(NTL_LONG_LONG_TYPE))

#define NTL_LL_TYPE NTL_LONG_LONG_TYPE

#elif (NTL_BITS_PER_LONG == 64 && defined(__GNUC__))

#define NTL_LL_TYPE __int128_t

#elif (NTL_BITS_PER_LONG == 32 && (defined(_MSC_VER) || defined(__BORLANDC__)))

#define NTL_LL_TYPE __int64

#elif (NTL_BITS_PER_LONG == 64 && (defined(_MSC_VER) || defined(__BORLANDC__)))

#define NTL_LL_TYPE __int128

#endif

#if (!defined(NTL_LL_TYPE))

#define NTL_LL_TYPE long long

#endif



#if (defined(NTL_UNSIGNED_LONG_LONG_TYPE))

#define NTL_ULL_TYPE NTL_UNSIGNED_LONG_LONG_TYPE

#elif (NTL_BITS_PER_LONG == 64 && defined(__GNUC__))

#define NTL_ULL_TYPE __uint128_t

#elif (NTL_BITS_PER_LONG == 32 && (defined(_MSC_VER) || defined(__BORLANDC__)))

#define NTL_ULL_TYPE unsigned __int64

#elif (NTL_BITS_PER_LONG == 64 && (defined(_MSC_VER) || defined(__BORLANDC__)))

#define NTL_ULL_TYPE unsigned __int128

#endif

#if (!defined(NTL_ULL_TYPE))

#define NTL_ULL_TYPE unsigned long long

#endif


#ifdef NTL_HAVE_LL_TYPE

typedef NTL_LL_TYPE _ntl_longlong;
typedef NTL_ULL_TYPE _ntl_ulonglong;
// typenames are more convenient than macros

#else

#undef NTL_LL_TYPE
#undef NTL_ULL_TYPE
// prevent any use of these macros

class _ntl_longlong { private: _ntl_longlong() { } };
class _ntl_ulonglong { private: _ntl_ulonglong() { } };
// cannot create variables of these types


#endif

/********************************************************/



// Define an unsigned type with at least 32 bits
// there is no truly portable way to do this, yet...


#if (NTL_BITS_PER_INT >= 32)

typedef unsigned int _ntl_uint32; // 32-bit word
#define NTL_BITS_PER_INT32 NTL_BITS_PER_INT

#else

// NOTE: C++ standard guarntees longs ar at least 32-bits wide,
// and this is also explicitly checked at builod time

typedef unsigned long _ntl_uint32; // 32-bit word
#define NTL_BITS_PER_INT32 NTL_BITS_PER_LONG

#endif



// The usual token pasting stuff...

#define NTL_PASTE_TOKENS2(a,b) a ## b
#define NTL_PASTE_TOKENS(a,b) NTL_PASTE_TOKENS2(a,b)

#define NTL_STRINGIFY(x) NTL_STRINGIFY_AUX(x)
#define NTL_STRINGIFY_AUX(x) #x






#define NTL_OVFBND (1L << (NTL_BITS_PER_LONG-4))

/*
 * NTL_OVFBND is the general bound used throughout NTL to keep various
 * integer values comfortably bounded away from an integer overflow
 * condition.  Do not change this value!
 */





#if ((NTL_BITS_PER_SIZE_T-1) < (NTL_BITS_PER_LONG-4))
#define NTL_OVFBND1 (1L << (NTL_BITS_PER_SIZE_T-1))
#else
#define NTL_OVFBND1 NTL_OVFBND
#endif

/*
 * NTL_OVFBND1 is a smaller bound than NTL_OVF when size_t is
 * narrower than long.  This prevents overflow on calls to malloc
 * and realloc.
 */






#define NTL_OVERFLOW(n, a, b) \
   (((b) >= NTL_OVFBND) || (((long) (n)) > 0 && (((a) >= NTL_OVFBND) || \
    (((long) (n)) >= (NTL_OVFBND-((long)(b))+((long)(a))-1)/((long)(a))))))

/*
 * NTL_OVERFLOW(n, a, b) returns 1 if n*a + b >= NTL_OVFBND,
 * and returns 0 otherwise.  The value n is effectively treated as type long,
 * while the values a and b may be *any* integral type.  It is assumed that
 * n >= 0, a > 0, and b >= 0.  Care is taken to ensure that overflow does
 * not occur. If a and b are constants, and n has no side effects,
 * a good optimizing compiler will * translate this into a single test 
 * of the form n >= c, where c is a constant.
 */






#define NTL_OVERFLOW1(n, a, b) \
   (((b) >= NTL_OVFBND1) || (((long) (n)) > 0 && (((a) >= NTL_OVFBND1) || \
    (((long) (n)) >= (NTL_OVFBND1-((long)(b))+((long)(a))-1)/((long)(a))))))

/*
 * NTL_OVERFLOW1 is the same as NTL_OVERFLOW, except that it uses the
 * bound NTL_OVFBND1 instead of NTL_OVFBND.
 */




#ifdef NTL_TEST_EXCEPTIONS

extern unsigned long exception_counter;

#define NTL_BASIC_MALLOC(n, a, b) \
   (NTL_OVERFLOW1(n, a, b) ? ((void *) 0) : \
    ((void *) malloc(((long)(n))*((long)(a)) + ((long)(b)))))

#define NTL_MALLOC(n, a, b) \
   (--exception_counter == 0 ? (void *) 0 : NTL_BASIC_MALLOC(n, a, b))

#else

#define NTL_MALLOC(n, a, b) \
   (NTL_OVERFLOW1(n, a, b) ? ((void *) 0) : \
    ((void *) malloc(((long)(n))*((long)(a)) + ((long)(b)))))


#endif

/*
 * NTL_MALLOC(n, a, b) returns 0 if a*n + b >= NTL_OVFBND1, and otherwise
 * returns malloc(n*a + b). 
 * The programmer must ensure that the name "malloc" is visible
 * at the point in the source code where this macro is expanded.
 */


#ifdef NTL_TEST_EXCEPTIONS

#define NTL_BASIC_SNS_MALLOC(n, a, b) \
   (NTL_OVERFLOW1(n, a, b) ? ((void *) 0) : \
    ((void *) NTL_SNS malloc(((long)(n))*((long)(a)) + ((long)(b)))))


#define NTL_SNS_MALLOC(n, a, b) \
   (--exception_counter == 0 ? (void *) 0 : NTL_BASIC_SNS_MALLOC(n, a, b))


#else

#define NTL_SNS_MALLOC(n, a, b) \
   (NTL_OVERFLOW1(n, a, b) ? ((void *) 0) : \
    ((void *) NTL_SNS malloc(((long)(n))*((long)(a)) + ((long)(b)))))

#endif

/*
 * NTL_SNS_MALLOC is the same as NTL_MALLOC, except that the call
 * to malloc is prefixed by NTL_SNS.
 */








#define NTL_REALLOC(p, n, a, b) \
   (NTL_OVERFLOW1(n, a, b) ? ((void *) 0) : \
    ((void *) realloc((p), ((long)(n))*((long)(a)) + ((long)(b)))))

/*
 * NTL_REALLOC(n, a, b) returns 0 if a*n + b >= NTL_OVFBND1, and otherwise
 * returns realloc(p, n*a + b).
 * The programmer must ensure that the name "realloc" is visible
 * at the point in the source code where this macro is expanded.
 */






#define NTL_SNS_REALLOC(p, n, a, b) \
   (NTL_OVERFLOW1(n, a, b) ? ((void *) 0) : \
    ((void *) NTL_SNS realloc((p), ((long)(n))*((long)(a)) + ((long)(b)))))

/*
 * NTL_SNS_REALLOC is the same as NTL_REALLOC, except that the call
 * to realloc is prefixed by NTL_SNS.
 */





#define NTL_MAX_ALLOC_BLOCK (40000)

/*
 * NTL_MAX_ALLOC_BLOCK is the number of bytes that are allocated in
 * a single block in a number of places throughout NTL (for
 * vec_ZZ_p, ZZVec, vec_GF2X, and GF2XVec).
 */


#define NTL_ULONG_TO_LONG(a) \
   ((((unsigned long) a) >> (NTL_BITS_PER_LONG-1)) ? \
    (((long) (((unsigned long) a) - ((unsigned long) NTL_MIN_LONG))) + \
       NTL_MIN_LONG) : \
    ((long) a))

/* 
 * This macro converts from unsigned long to signed long.  It is portable
 * among platforms for which a long has a 2's complement representation
 * of the same width as an unsigned long.  While it avoids assumptions
 * about the behavior of non-standard conversions,  a good optimizing
 * compiler should turn it into the identity function.
 */


#define NTL_UINT_TO_INT(a) \
   ((((unsigned int) a) >> (NTL_BITS_PER_INT-1)) ? \
    (((int) (((unsigned int) a) - ((unsigned int) NTL_MIN_INT))) + \
       NTL_MIN_INT) : \
    ((int) a))

/* 
 * This macro converts from unsigned int to signed int.  It is portable
 * among platforms for which an int has a 2's complement representation
 * of the same width as an unsigned int.  While it avoids assumptions
 * about the behavior of non-standard conversions,  a good optimizing
 * compiler should turn it into the identity function.
 */


#ifdef NTL_THREADS

#define NTL_THREAD_LOCAL thread_local 

#ifdef __GNUC__
#define NTL_CHEAP_THREAD_LOCAL __thread
#else
#define NTL_CHEAP_THREAD_LOCAL thread_local
#endif

#else

#define NTL_THREAD_LOCAL 
#define NTL_CHEAP_THREAD_LOCAL 

#endif


#define NTL_RELEASE_THRESH (128)

/*
 * threshold for releasing scratch memory.
 */



long _ntl_IsFinite(double *p);
/* This forces a double into memory, and tests if it is "normal";
   that means, not NaN, not +/- infinity, not denormalized, etc.
   Forcing into memory is sometimes necessary on machines 
   with "extended" double precision registers (e.g., Intel x86s)
   to force the standard IEEE format. */

void _ntl_ForceToMem(double *p);
/* This is do-nothing routine that has the effect of forcing
   a double into memory (see comment above). */

double _ntl_ldexp(double x, long e);


#define NTL_DEFINE_SWAP(T)\
inline void _ntl_swap(T& a, T& b)\
{\
   T t = a; a = b; b = t;\
}

//NTL_DEFINE_SWAP(long)
//NTL_DEFINE_SWAP(int)
//NTL_DEFINE_SWAP(short)
//NTL_DEFINE_SWAP(char)
//
//NTL_DEFINE_SWAP(unsigned long)
//NTL_DEFINE_SWAP(unsigned int)
//NTL_DEFINE_SWAP(unsigned short)
//NTL_DEFINE_SWAP(unsigned char)
//
//NTL_DEFINE_SWAP(double)
//NTL_DEFINE_SWAP(float)

//   
//template<class T>
//void _ntl_swap(T*& a, T*& b)
//{
//   T* t = a; a = b; b = t;
//}

/* These are convenience routines.  I don't want it to overload
   the std library's swap function, nor do I want to rely on the latter,
   as the C++ standard is kind of broken on the issue of where
   swap is defined. And I also only want it defined for built-in types.
 */




// The following is for aligning small local arrays
// Equivalent to type x[n], but aligns to align bytes
// Only works for POD types
// NOTE: the gcc aligned attribute might work, but there is
// some chatter on the web that this was (at some point) buggy.
// Not clear what the current status is.
// Anyway, this is only intended for use with gcc on intel
// machines, so it should be OK.


#define NTL_ALIGNED_LOCAL_ARRAY(align, x, type, n) \
   char x##__ntl_hidden_variable_storage[n*sizeof(type)+align]; \
   type *x = (type *) ((&x##__ntl_hidden_variable_storage[0]) + \
                       ((-((unsigned long) (&x##__ntl_hidden_variable_storage[0]))) %\
                       (unsigned long)(align))) \


#define NTL_AVX_BYTE_ALIGN (32)
#define NTL_AVX_DBL_ALIGN (NTL_AVX_BYTE_ALIGN/long(sizeof(double)))

#define NTL_AVX_LOCAL_ARRAY(x, type, n) NTL_ALIGNED_LOCAL_ARRAY(NTL_AVX_BYTE_ALIGN, x, type, n)

#define NTL_DEFAULT_ALIGN (64)
// this should be big enough to satisfy any SIMD instructions,
// and it should also be as big as a cache line


   

#endif

