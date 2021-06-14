/* src/config.h.  Generated from config.h.in by configure.  */
/* src/config.h.in.  Generated from configure.ac by autoheader.  */

/* Define if you want to enable Beagle */
/* #undef BEAGLE_ENABLED */

/* Define to enable Beagle v3.1 extensions */
/* #undef BEAGLE_V3_ENABLED */

/* Compiler vendor */
#define COMPILER_VENDOR "gnu"

/* Compiler version */
#define COMPILER_VERSION "7.5.0"

/* AVX code path should not be compiled. */
/* #undef DISABLE_AVX */

/* FMA code path should not be compiled. */
/* #undef DISABLE_FMA */

/* SSE code path should not be compiled. */
/* #undef DISABLE_SSE */

/* Define to 1 to support Advanced Bit Manipulation */
/* #undef HAVE_ABM */

/* Define to 1 to support Multi-Precision Add-Carry Instruction Extensions */
/* #undef HAVE_ADX */

/* Define to 1 to support Advanced Encryption Standard New Instruction Set
   (AES-NI) */
#define HAVE_AES 1

/* Support Altivec instructions */
/* #undef HAVE_ALTIVEC */

/* Define to 1 to support Advanced Vector Extensions */
#define HAVE_AVX 1

/* Define to 1 to support Advanced Vector Extensions 2 */
/* #undef HAVE_AVX2 */

/* Define to 1 to support AVX-512 Byte and Word Instructions */
/* #undef HAVE_AVX512_BW */

/* Define to 1 to support AVX-512 Conflict Detection Instructions */
/* #undef HAVE_AVX512_CD */

/* Define to 1 to support AVX-512 Doubleword and Quadword Instructions */
/* #undef HAVE_AVX512_DQ */

/* Define to 1 to support AVX-512 Exponential & Reciprocal Instructions */
/* #undef HAVE_AVX512_ER */

/* Define to 1 to support AVX-512 Foundation Extensions */
/* #undef HAVE_AVX512_F */

/* Define to 1 to support AVX-512 Integer Fused Multiply Add Instructions */
/* #undef HAVE_AVX512_IFMA */

/* Define to 1 to support AVX-512 Conflict Prefetch Instructions */
/* #undef HAVE_AVX512_PF */

/* Define to 1 to support AVX-512 Vector Byte Manipulation Instructions */
/* #undef HAVE_AVX512_VBMI */

/* Define to 1 to support AVX-512 Vector Length Extensions */
/* #undef HAVE_AVX512_VL */

/* Define to 1 to support Bit Manipulation Instruction Set 1 */
/* #undef HAVE_BMI1 */

/* Define to 1 to support Bit Manipulation Instruction Set 2 */
/* #undef HAVE_BMI2 */

/* Define to 1 if you have the <float.h> header file. */
#define HAVE_FLOAT_H 1

/* Define to 1 if you have the `floor' function. */
#define HAVE_FLOOR 1

/* Define to 1 to support Fused Multiply-Add Extensions 3 */
/* #undef HAVE_FMA3 */

/* Define to 1 to support Fused Multiply-Add Extensions 4 */
/* #undef HAVE_FMA4 */

/* Define to 1 if you have the <history.h> header file. */
/* #undef HAVE_HISTORY_H */

/* Define to 1 if you have the <inttypes.h> header file. */
#define HAVE_INTTYPES_H 1

/* Define to 1 if you have the `m' library (-lm). */
#define HAVE_LIBM 1

/* Define if you have a readline compatible library */
/* #undef HAVE_LIBREADLINE */

/* Define to 1 if you have the <limits.h> header file. */
#define HAVE_LIMITS_H 1

/* Define to 1 if you have the <memory.h> header file. */
#define HAVE_MEMORY_H 1

/* Define to 1 if you have the `memset' function. */
#define HAVE_MEMSET 1

/* Define to 1 to support Multimedia Extensions */
#define HAVE_MMX 1

/* Define to 1 to support Memory Protection Extensions */
/* #undef HAVE_MPX */

/* Define to 1 if you have the `pow' function. */
#define HAVE_POW 1

/* Define to 1 to support Prefetch Vector Data Into Caches WT1 */
/* #undef HAVE_PREFETCHWT1 */

/* Define to 1 to support Digital Random Number Generator */
#define HAVE_RDRND 1

/* Define to 1 if you have the <readline.h> header file. */
/* #undef HAVE_READLINE_H */

/* Define if your readline library has \`add_history' */
/* #undef HAVE_READLINE_HISTORY */

/* Define to 1 if you have the <readline/history.h> header file. */
/* #undef HAVE_READLINE_HISTORY_H */

/* Define to 1 if you have the <readline/readline.h> header file. */
/* #undef HAVE_READLINE_READLINE_H */

/* Define to 1 to support Secure Hash Algorithm Extension */
/* #undef HAVE_SHA */

/* Define to 1 if you have the `sqrt' function. */
#define HAVE_SQRT 1

/* Define to 1 to support Streaming SIMD Extensions */
#define HAVE_SSE 1

/* Define to 1 to support Streaming SIMD Extensions */
#define HAVE_SSE2 1

/* Define to 1 to support Streaming SIMD Extensions 3 */
#define HAVE_SSE3 1

/* Define to 1 to support Streaming SIMD Extensions 4.1 */
#define HAVE_SSE4_1 1

/* Define to 1 to support Streaming SIMD Extensions 4.2 */
#define HAVE_SSE4_2 1

/* Define to 1 to support AMD Streaming SIMD Extensions 4a */
/* #undef HAVE_SSE4a */

/* Define to 1 to support Supplemental Streaming SIMD Extensions 3 */
#define HAVE_SSSE3 1

/* Define to 1 if you have the <stdint.h> header file. */
#define HAVE_STDINT_H 1

/* Define to 1 if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define to 1 if you have the `strchr' function. */
#define HAVE_STRCHR 1

/* Define to 1 if you have the <strings.h> header file. */
#define HAVE_STRINGS_H 1

/* Define to 1 if you have the <string.h> header file. */
#define HAVE_STRING_H 1

/* Define to 1 if you have the <sys/stat.h> header file. */
#define HAVE_SYS_STAT_H 1

/* Define to 1 if you have the <sys/types.h> header file. */
#define HAVE_SYS_TYPES_H 1

/* Define to 1 if you have the <unistd.h> header file. */
#define HAVE_UNISTD_H 1

/* Support VSX instructions */
/* #undef HAVE_VSX */

/* Define to 1 to support eXtended Operations Extensions */
/* #undef HAVE_XOP */

/* Define to 1 if the system has the type `_Bool'. */
#define HAVE__BOOL 1

/* Host CPU type */
#define HOST_CPU "x86_64"

/* Host triplet */
#define HOST_TYPE "x86_64-unknown-linux-gnu"

/* Define if you want to enable MPI */
/* #undef MPI_ENABLED */

/* Name of package */
#define PACKAGE "mrbayes"

/* Define to the address where bug reports for this package should be sent. */
#define PACKAGE_BUGREPORT ""

/* Define to the full name of this package. */
#define PACKAGE_NAME "MrBayes"

/* Define to the full name and version of this package. */
#define PACKAGE_STRING "MrBayes 3.2.7a"

/* Define to the one symbol short name of this package. */
#define PACKAGE_TARNAME "mrbayes"

/* Define to the home page for this package. */
#define PACKAGE_URL ""

/* Define to the version of this package. */
#define PACKAGE_VERSION "3.2.7a"

/* Define to 1 if you have the ANSI C header files. */
#define STDC_HEADERS 1

/* Version number of package */
#define VERSION "3.2.7a"

/* Define to `unsigned int' if <sys/types.h> does not define. */
/* #undef size_t */
