AC_INIT([libgim], [0.1.0], [danny@nerdcruft.net])

AC_CANONICAL_HOST

## We remove CXXFLAGS as autotools inserts them /AFTER/ our own CXXFLAGS,
## thus overriding any variables that both set (specifically we're concerned
## about optimisation flags).
CXXFLAGS=""
AC_PROG_CXX
AC_LANG([C++])
AC_PROG_LIBTOOL
AM_PROG_CC_C_O

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE 
AM_CONFIG_HEADER(config.h)


##
## Warnings
WARNING_FLAGS=""

AS_COMPILER_FLAG([-Wall], 		[WARNING_FLAGS="$WARNING_FLAGS -Wall"])
AS_COMPILER_FLAG([-Wextra], 		[WARNING_FLAGS="$WARNING_FLAGS -Wextra"])
AS_COMPILER_FLAG([-Wno-parentheses],    [WARNING_FLAGS="$WARNING_FLAGS -Wno-parentheses"])

AS_COMPILER_FLAG([-Wpointer-arith],	[WARNING_FLAGS="$WARNING_FLAGS -Wpointer-arith"])
AS_COMPILER_FLAG([-Wcast-qual],         [WARNING_FLAGS="$WARNING_FLAGS -Wcast-qual"])
AS_COMPILER_FLAG([-Wcast-align],        [WARNING_FLAGS="$WARNING_FLAGS -Wcast-align"])

AS_COMPILER_FLAG([-Wsign-compare],      [WARNING_FLAGS="$WARNING_FLAGS -Wsign-compare"])
AS_COMPILER_FLAG([-Wsign-conversion],   [WARNING_FLAGS="$WARNING_FLAGS -Wsign-conversion"])
AS_COMPILER_FLAG([-Wtype-limits],       [WARNING_FLAGS="$WARNING_FLAGS -Wtype-limits"])
AS_COMPILER_FLAG([-Wfloat-equal],       [WARNING_FLAGS="$WARNING_FLAGS -Wfloat-equal"])

#AS_COMPILER_FLAG([-Wswitch-default],    [COMMON_CFLAGS="$COMMON_CFLAGS -Wswitch-default"])
AS_COMPILER_FLAG([-Wswitch-enum],       [COMMON_CFLAGS="$COMMON_CFLAGS -Wswitch-enum"])

AS_COMPILER_FLAG([-Wunsafe-loop-optimizations],
                 [WARNING_FLAGS="$WARNING_FLAGS -Wunsafe-loop-optimizations"])

AS_COMPILER_FLAG([-Wunused-parameter],  [WARNING_FLAGS="$WARNING_FLAGS -Wunused-parameter"])
AS_COMPILER_FLAG([-Wunused-but-set-variable],    [WARNING_FLAGS="$WARNING_FLAGS -Wunused-parameter"])
AS_COMPILER_FLAG([-Wshadow],            [WARNING_FLAGS="$WARNING_FLAGS -Wshadow"])
AS_COMPILER_FLAG([-Wredundant-decls],   [WARNING_FLAGS="$WARNING_FLAGS -Wredundant-decls"])

AS_COMPILER_FLAG([-pedantic],           [COMMON_CFLAGS="$COMMON_CFLAGS -pedantic"])

##
## Compilation
AS_COMPILER_FLAG([-fno-common ],        [COMMON_CFLAGS="$COMMON_CFLAGS -fno-common "])
AS_COMPILER_FLAG([-fno-nonansi-builtins],
                 [COMMON_CFLAGS="$COMMON_CFLAGS -fno-nonansi-builtins"])
AS_COMPILER_FLAG([-fno-rtti],           [COMMON_CFLAGS="$COMMON_CFLAGS -fno-rtti"])

AS_COMPILER_FLAG([-ggdb],               [COMMON_CFLAGS="$COMMON_CFLAGS -ggdb"])

##
## Compiler features
AC_C_CONST
AC_C_RESTRICT
AC_C_INLINE

##
## Architecture features
AC_C_BIGENDIAN

##
## platform features
AC_FUNC_MMAP

AC_CHECK_HEADER([execinfo.h], [break], [AC_MSG_ERROR([Missing backtrace support])])

AM_CONDITIONAL([HAVE_EXECINFO], [test x$ac_cv_header_execinfo_h = "xyes"])


##
## Documentation

##
## Required packages
CHECK_RAGEL

AX_BOOST_BASE([1.46])
AX_BOOST_FILESYSTEM
AX_BOOST_SYSTEM

## Optional packages

## Output

COMMON_CXXFLAGS="-O0 -g -std=c++0x -include config.h $WARNING_FLAGS $COMMON_CFLAGS"
COMMON_CFLAGS="-O0 -g -std=c99   -include config.h $WARNING_FLAGS $COMMON_CFLAGS"
COMMON_LDFLAGS=""

AC_SUBST(COMMON_CFLAGS)
AC_SUBST(COMMON_CXXFLAGS)

AC_OUTPUT([
    Doxyfile
    Makefile
    test/Makefile
])
