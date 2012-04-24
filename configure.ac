AC_INIT([libgim], [0.1.0], [danny@nerdcruft.net])

AC_CANONICAL_HOST

## We remove CXXFLAGS as autotools inserts them /AFTER/ our own CXXFLAGS,
## thus overriding any variables that both set (specifically we're concerned
## about optimisation flags).
CXXFLAGS="-g"
AC_PROG_CXX
AC_LANG([C++])
LT_INIT
AM_PROG_CC_C_O

AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_HEADERS([config.h])

AM_INIT_AUTOMAKE([1.11 silent-rules])
AM_SILENT_RULES([yes])


AC_ARG_ENABLE([debugging],
              [AS_HELP_STRING([--enable-debugging],
                              [enables developer debugging support])],
              [ case "${enableval}" in
                    yes) ac_cv_debugging=yes ;;
                     no) ac_cv_debugging=no ;;
                      *) AC_MSG_ERROR([bad value for --enable-debugging=[yes|no]]) ;;
              esac ],
              [ac_cv_debugging=no])

AC_ARG_ENABLE([mudflap],
              [AS_HELP_STRING([--enable-mudflap],
                              [enable mudflap runtime debugging])],
              [ case "${enableval}" in
                    yes) ac_cv_mudflap=yes ;;
                     no) ac_cv_mudflap=no ;;
                      *) AC_MSG_ERROR([bad value for --enable-mudflap[yes|no]]) ;;
              esac ],
              [ac_cv_mudflap=no])

##
## Warnings
WARNING_FLAGS=""

AS_CXX_COMPILER_FLAG([-Wall], 		[WARNING_FLAGS="$WARNING_FLAGS -Wall"])
AS_CXX_COMPILER_FLAG([-Wextra], 		[WARNING_FLAGS="$WARNING_FLAGS -Wextra"])
AS_CXX_COMPILER_FLAG([-Wno-parentheses],    [WARNING_FLAGS="$WARNING_FLAGS -Wno-parentheses"])

AS_CXX_COMPILER_FLAG([-Wpointer-arith],	[WARNING_FLAGS="$WARNING_FLAGS -Wpointer-arith"])
AS_CXX_COMPILER_FLAG([-Wcast-qual],         [WARNING_FLAGS="$WARNING_FLAGS -Wcast-qual"])
AS_CXX_COMPILER_FLAG([-Wcast-align],        [WARNING_FLAGS="$WARNING_FLAGS -Wcast-align"])

AS_CXX_COMPILER_FLAG([-Wsign-compare],      [WARNING_FLAGS="$WARNING_FLAGS -Wsign-compare"])
AS_CXX_COMPILER_FLAG([-Wsign-conversion],   [WARNING_FLAGS="$WARNING_FLAGS -Wsign-conversion"])
AS_CXX_COMPILER_FLAG([-Wtype-limits],       [WARNING_FLAGS="$WARNING_FLAGS -Wtype-limits"])
AS_CXX_COMPILER_FLAG([-Wfloat-equal],       [WARNING_FLAGS="$WARNING_FLAGS -Wfloat-equal"])

#AS_CXX_COMPILER_FLAG([-Wswitch-default],    [COMMON_CFLAGS="$COMMON_CFLAGS -Wswitch-default"])
AS_CXX_COMPILER_FLAG([-Wswitch-enum],       [COMMON_CFLAGS="$COMMON_CFLAGS -Wswitch-enum"])

AS_CXX_COMPILER_FLAG([-Wunsafe-loop-optimizations],
                 [WARNING_FLAGS="$WARNING_FLAGS -Wunsafe-loop-optimizations"])

AS_CXX_COMPILER_FLAG([-Wunused-parameter],  [WARNING_FLAGS="$WARNING_FLAGS -Wunused-parameter"])
AS_CXX_COMPILER_FLAG([-Wunused-but-set-variable],    [WARNING_FLAGS="$WARNING_FLAGS -Wunused-parameter"])
AS_CXX_COMPILER_FLAG([-Wshadow],            [WARNING_FLAGS="$WARNING_FLAGS -Wshadow"])
AS_CXX_COMPILER_FLAG([-Wredundant-decls],   [WARNING_FLAGS="$WARNING_FLAGS -Wredundant-decls"])

AS_CXX_COMPILER_FLAG([-pedantic],           [COMMON_CFLAGS="$COMMON_CFLAGS -pedantic"])

##
## Compilation
AS_CXX_COMPILER_FLAG([-fno-common ],        [COMMON_CFLAGS="$COMMON_CFLAGS -fno-common "])
AS_CXX_COMPILER_FLAG([-fno-nonansi-builtins],
                 [COMMON_CFLAGS="$COMMON_CFLAGS -fno-nonansi-builtins"])
AS_CXX_COMPILER_FLAG([-fno-rtti],           [COMMON_CFLAGS="$COMMON_CFLAGS -fno-rtti"])

AS_CXX_COMPILER_FLAG([-ggdb],               [COMMON_CFLAGS="$COMMON_CFLAGS -ggdb"])

##
## Compiler features
AC_C_CONST
AC_C_RESTRICT
AC_C_INLINE

##
## Architecture features
AC_C_BIGENDIAN

##
## Useful headers or platform features
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

##
## platform features
COMMON_CFLAGS="$COMMON_CFLAGS -D_GNU_SOURCE"

AC_FUNC_MMAP

AC_CHECK_HEADER([execinfo.h], [break])
AM_CONDITIONAL([HAVE_EXECINFO], [test x$ac_cv_header_execinfo_h = "xyes"])

AC_SEARCH_LIBS([clock_gettime], [rt])
if [[ "x$ac_cv_search_clock_gettime" == "x-*" ]]; then
    COMMON_LDFLAGS="$COMMON_LDFLAGS $ac_cv_search_clock_gettime"
fi
    

##
## Debug features
if test "x$ac_cv_debugging" = "xyes"; then
    COMMON_CFLAGS="$COMMON_CFLAGS -O0 -D_GLIBCXX_DEBUG"

    if test "x$ac_cv_mudflap" = "xyes"; then
        AS_CXX_COMPILER_FLAG([-fmudflapth], [
            COMMON_CFLAGS="$COMMON_CFLAGS -fmudflapth";
            COMMON_LDFLAGS="$COMMON_LDFLAGS -lmudflapth"
        ])
    fi

    COMMON_CFLAGS="$COMMON_CFLAGS -D_FORTIFY_SOURCE=2"
else
    COMMON_CFLAGS="$COMMON_CFLAGS -O2"
fi

##
## Documentation

##
## Required packages
CHECK_RAGEL([test/ragel.c.rl])


AX_BOOST_BASE([1.42], [], [AC_MSG_ERROR([Boost version >= 1.42 required])])
AC_SUBST(BOOST_CPPFLAGS)
AC_SUBST(BOOST_LDFLAGS)

# boost-system isn't a hard requirement, it's only really used to fulfill
# some other dependency I've since forgotten about...
AX_BOOST_SYSTEM

AX_BOOST_FILESYSTEM
AC_SUBST(BOOST_FILESYSTEM_LIB)

if test "x$ax_cv_boost_filesystem" != "xyes"; then
    AC_MSG_ERROR("boost-filesystem is a hard requirement")
fi

## Optional packages

## Output

COMMON_CXXFLAGS="-g -std=c++0x -pipe -include config.h $WARNING_FLAGS $COMMON_CFLAGS"
COMMON_CFLAGS="-g -std=c99 -pipe -include config.h $WARNING_FLAGS $COMMON_CFLAGS"
COMMON_LDFLAGS="$COMMON_LDFLAGS "

AC_SUBST(COMMON_CFLAGS)
AC_SUBST(COMMON_CXXFLAGS)
AC_SUBST(COMMON_LDFLAGS)

AC_CONFIG_FILES([
    Doxyfile
    Makefile
    test/Makefile
])
AC_OUTPUT
