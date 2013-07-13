AC_INIT([libgim], [0.1.0], [danny@nerdcruft.net])
## Explicitly set an empty CXXFLAGS if not present to prevent AC_PROG_CXX from
## generating a default -O2. This allows us to manually select -O0 when
## debugging is enabled.
: ${CXXFLAGS=""}

AC_CANONICAL_HOST

AC_PROG_CXX
AC_LANG([C++])
LT_INIT
AM_PROG_CC_C_O

AC_CONFIG_AUX_DIR([build-aux])
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

AS_CXX_COMPILER_FLAG([-Wswitch-default],    [COMMON_CFLAGS="$COMMON_CFLAGS -Wswitch-default"])
#AS_CXX_COMPILER_FLAG([-Wswitch-enum],       [COMMON_CFLAGS="$COMMON_CFLAGS -Wswitch-enum"])

#AS_CXX_COMPILER_FLAG([-Wzero-as-null-pointer-constant], [WARNING_FLAGS="$WARNING_FLAGS -Wzero-as-null-pointer-constant"])

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
##AS_CXX_COMPILER_FLAG([-fno-rtti],           [COMMON_CFLAGS="$COMMON_CFLAGS -fno-rtti"])

AS_CXX_COMPILER_FLAG([-fno-deduce-init-list], [COMMON_CFLAGS="$COMMON_CFLAGS -fno-deduce-init-list "])


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
#AC_DEFINE([_GNU_SOURCE], [], [Description])

case $host_os in
    mingw32)
        AM_CONDITIONAL([PLATFORM_WIN32], [true])
        AM_CONDITIONAL([PLATFORM_LINUX], [false])

        AC_CHECK_HEADERS([winsock2.h ws2tcpip.h])  
        AC_SUBST([NET_LIBS], [-lws2_32])

        #COMMON_LDFLAGS="$COMMON_LDFLAGS -ldbghelp"
        ;;

    linux-gnu)
        AM_CONDITIONAL([PLATFORM_WIN32], [false])
        AM_CONDITIONAL([PLATFORM_LINUX], [true])
        ;;

    *)
        AC_ERROR([Unknown host_os])
        ;;
esac

AC_FUNC_MMAP

AC_CHECK_FUNC([strndup], [
	AC_DEFINE([HAVE_STRNDUP], [], [strndup appears to be present])
])

AC_CHECK_HEADER([execinfo.h], [break])
AM_CONDITIONAL([HAVE_EXECINFO], [test x$ac_cv_header_execinfo_h = "xyes"])

AC_SEARCH_LIBS([clock_gettime], [rt])
if [[ "x$ac_cv_search_clock_gettime" == "x-*" ]]; then
    COMMON_LDFLAGS="$COMMON_LDFLAGS $ac_cv_search_clock_gettime"
fi


##
## Debug features
if test "x$ac_cv_debugging" = "xyes"; then
    COMMON_CFLAGS="$COMMON_CFLAGS -O0"

    if test "x$ac_cv_mudflap" = "xyes"; then
        AS_CXX_COMPILER_FLAG([-fmudflapth], [
            COMMON_CFLAGS="$COMMON_CFLAGS -fmudflapth";
            COMMON_LDFLAGS="$COMMON_LDFLAGS -lmudflapth"
        ])
    fi

    AC_DEFINE([ENABLE_DEBUGGING], [], [Description])
    AC_DEFINE([_GLIBCXX_DEBUG], [], [Description])

else
    ## Use 'none' LTO partitioning to avoid discarded section linker errors
    ## against boost
    COMMON_CFLAGS="$COMMON_CFLAGS -O2 -flto=8 -flto-partition=none"
    COMMON_LDFLAGS="$COMMON_LDFLAGS -flto=8 -flto-partition=none"
fi

case $host_os in
    mingw32)
        COMMON_CFLAGS="$COMMON_CFLAGS -gstabs"
        ;;

    linux-gnu)
        COMMON_CFLAGS="$COMMON_CFLAGS -g"
        AS_CXX_COMPILER_FLAG([-ggdb], [COMMON_CFLAGS="$COMMON_CFLAGS -ggdb"])
        ;;

    *)
        AC_ERROR([Unknown host_os])
        ;;
esac

case $host_cpu in
    x86_64)
        OPTIMISATION_FLAGS="$OPTIMISATION_FLAGS -mtune=generic"
        ;;

    i686)
        OPTIMISATION_FLAGS="$OPTIMISATION_FLAGS -march=prescott -mtune=generic -mfpmath=sse"
        ;;

    *)
        AC_MSG_ERROR([Unknown target CPU])
        ;;
esac


##
## Documentation

##
## Required packages
CHECK_RAGEL([ip.cpp])


AX_BOOST_BASE([1.49], [], [AC_MSG_ERROR([Boost version >= 1.42 required])])
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
PKG_CHECK_MODULES([ZLIB], [zlib >= 1.2.0])

## Output

COMMON_CXXFLAGS="-std=c++0x -pipe -include config.h $WARNING_FLAGS $OPTIMISATION_FLAGS $COMMON_CFLAGS"
COMMON_LDFLAGS="$COMMON_LDFLAGS "

AC_SUBST(COMMON_CXXFLAGS)
AC_SUBST(COMMON_LDFLAGS)

AC_CONFIG_FILES([
    Doxyfile
    Makefile
    test/Makefile
])
AC_OUTPUT
