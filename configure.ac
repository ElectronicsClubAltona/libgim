AC_INIT([libgim], [0.1.0], [danny@nerdcruft.net])
## Explicitly set an empty CXXFLAGS if not present to prevent AC_PROG_CXX from
## generating a default -O2. This allows us to manually select -O0 when
## debugging is enabled.
: ${CXXFLAGS=""}

###############################################################################
## Build environment discovery

AC_CONFIG_AUX_DIR([build-aux])
AC_REQUIRE_AUX_FILE([tap-driver.sh])
AC_CONFIG_MACRO_DIR([m4])

AC_USE_SYSTEM_EXTENSIONS
AC_CANONICAL_HOST

LT_INIT
AC_LANG([C++])
AC_PROG_CXX
AC_PROG_CXXCPP
AC_PROG_RANLIB

AX_APPEND_COMPILE_FLAGS([-std=c++1y])

AM_INIT_AUTOMAKE([1.14 dist-bzip2 dist-xz foreign subdir-objects])
AM_SILENT_RULES([yes])

AC_CONFIG_HEADERS([config.h])

###############################################################################
## Major compilation options

AC_ARG_ENABLE([debugging],
    [AS_HELP_STRING([--enable-debugging], [enables developer debugging support])]
)

AC_ARG_ENABLE([sanitizer], [
    AS_HELP_STRING([--enable-sanitizer], [enable memory sanitizer])
])

###############################################################################
## Warnings

# Compound warnings
AX_APPEND_COMPILE_FLAGS([-Wall], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wextra], [], [-Werror])

# General warnings
AX_APPEND_COMPILE_FLAGS([-Wcast-align], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wcast-qual], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wfloat-conversion], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wfloat-equal], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wno-aggressive-loop-optimizations], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wnoexcept], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wnon-virtual-dtor], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wno-parentheses], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wpointer-arith], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wredundant-decls], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wshadow], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wsign-compare], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wsign-promo], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wstrict-aliasing], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wstrict-overflow], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wtype-limits], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wunused-but-set-variable], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wunused-parameter], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wuseless-cast], [], [-Werror])

# Required extensions
#AX_APPEND_COMPILE_FLAGS([-Wgnu-flexible-array-union-member], [], [-Werror])
#AX_APPEND_COMPILE_FLAGS([-Wno-c99-extensions], [], [-Werror])
#AX_APPEND_COMPILE_FLAGS([-Wno-gnu-zero-variadic-macro-arguments], [], [-Werror])
#AX_APPEND_COMPILE_FLAGS([-Wno-nested-anon-types], [], [-Werror])
#AX_APPEND_COMPILE_FLAGS([-Wno-vla-extension], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-Wno-vla], [], [-Werror])

# Excessive warnings
#AX_APPEND_COMPILE_FLAGS([-Wunsafe-loop-optimizations], [], [-Werror])
#AX_APPEND_COMPILE_FLAGS([-Winline], [], [-Werror])
#AX_APPEND_COMPILE_FLAGS([-Wconversion], [], [-Werror])
#AX_APPEND_COMPILE_FLAGS([-Wdouble-promotion], [], [-Werror])
#AX_APPEND_COMPILE_FLAGS([-Wold-style-cast], [], [-Werror])
#AX_APPEND_COMPILE_FLAGS([-Woverloaded-virtual], [], [-Werror])
#AX_APPEND_COMPILE_FLAGS([-Wsign-conversion], [], [-Werror])

# Strict warnings
AX_APPEND_COMPILE_FLAGS([-pedantic], [], [-Werror])

######################################################################
## Platform compilation helpers

AX_APPEND_COMPILE_FLAGS([-fno-common], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-fno-nonansi-builtins], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-fno-deduce-init-list], [], [-Werror])

###############################################################################
## Compiler features

AC_C_CONST
AC_C_RESTRICT
AC_C_INLINE

AC_COMPILE_IFELSE([AC_LANG_PROGRAM([struct A     { virtual void C (void) = 0;  };
                                    struct B : A { void C (void) override; }; ],
                                   [])],
                  [], [AC_DEFINE([override], [], [Pretend about override keyword support])])

AC_COMPILE_IFELSE([AC_LANG_PROGRAM([struct A final { }; ],
                                   [])],
                  [], [AC_DEFINE([final], [], [Pretend about final keyword support])])

###############################################################################
## Architecture features

AC_C_BIGENDIAN

###############################################################################
## Useful headers or platform features

AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

###############################################################################
## Platform features

AS_CASE([$host_os],
    [mingw32], [
        AM_CONDITIONAL([PLATFORM_WIN32], [true])
        AM_CONDITIONAL([PLATFORM_LINUX], [false])

        AC_CHECK_HEADERS([winsock2.h ws2tcpip.h])  
        AC_SUBST([NET_LIBS], [-lws2_32])
    ],

    [linux-gnu], [
        AM_CONDITIONAL([PLATFORM_WIN32], [false])
        AM_CONDITIONAL([PLATFORM_LINUX], [true])
    ],

    [AC_ERROR([Unknown host_os])]
)

AC_FUNC_MMAP

AC_CHECK_FUNC([strndup], [
    AC_DEFINE([HAVE_STRNDUP], [], [strndup appears to be present])
])

AC_CHECK_HEADER([execinfo.h], [break])
AM_CONDITIONAL([HAVE_EXECINFO], [test x$ac_cv_header_execinfo_h = "xyes"])

AC_SEARCH_LIBS([clock_gettime], [rt])
AS_IF([test "x$ac_cv_search_clock_gettime" == "x-*"], [
    AX_APPEND_LINK_FLAGS([$ac_cv_search_clock_gettime], [], [-Werror])
])

###############################################################################
## Debug features

AS_IF([test "x$enable_debugging" = "xyes"], [
    AS_IF([test "x$enable_sanitizer" = "xyes"], [
        AX_APPEND_COMPILE_FLAGS([-fsanitize=address], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-fsanitize=undefined], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-fsanitize=leak], [], [-Werror])
        AX_APPEND_LINK_FLAGS([-fsanitize=address], [], [-Werror])
        AX_APPEND_LINK_FLAGS([-fsanitize=undefined], [], [-Werror])
        AX_APPEND_LINK_FLAGS([-fsanitize=leak], [], [-Werror])
    ])

    AX_APPEND_COMPILE_FLAGS([-ggdb], [], [-Werror])
    AX_APPEND_LINK_FLAGS([-rdynamic])

    AC_DEFINE([ENABLE_DEBUGGING], [], [Debugging support enabled], [], [-Werror])
    AC_DEFINE([_GLIBCXX_DEBUG],   [], [Use glibcxx debugging mode], [], [-Werror])

    AX_APPEND_COMPILE_FLAGS([-O0], [], [-Werror])
], [
    AX_APPEND_COMPILE_FLAGS([-O2], [], [-Werror])

    # mingw LTO support currently appears to be broken
    AS_IF([test x"${host_os}" != x"mingw32"], [
        AX_APPEND_COMPILE_FLAGS([-flto])
        AX_APPEND_LINK_FLAGS([-fuse-linker-plugin], [], [-Werror])
    ])
])

AS_CASE([$host_os],
    [mingw32], [
        AX_APPEND_COMPILE_FLAGS([-gstabs], [], [-Werror])
    ],

    [linux-gnu], [
        AX_APPEND_COMPILE_FLAGS([-g], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-ggdb], [], [-Werror])
    ],

    [AC_MSG_WARN([Unknown host_os])]
)

###############################################################################
## Documentation

###############################################################################
## Required packages

CHECK_RAGEL([ip.cpp])

AX_BOOST_BASE([1.53], [], [AC_MSG_ERROR([Boost version >= 1.53 required])])

# boost-system isn't a hard requirement, it's only really used to fulfill
# some other dependency I've since forgotten about...
AX_BOOST_SYSTEM

AX_BOOST_THREAD
AS_IF([test "x$ax_cv_boost_thread" != "xyes"], [
    AC_MSG_ERROR("boost-thread is a hard requirement")
])

AX_BOOST_FILESYSTEM
AS_IF([test "x$ax_cv_boost_filesystem" != "xyes"], [
    AC_MSG_ERROR("boost-filesystem is a hard requirement")
])


###############################################################################
## Optional packages

PKG_CHECK_MODULES([ZLIB], [zlib >= 1.2.0])
AC_SUBST(ZLIB_CFLAGS)
AC_SUBST(ZLIB_LIBS)

###############################################################################
## Performance and build optimisations

AS_CASE([$host_cpu],
    [x86_64], [
        AX_APPEND_COMPILE_FLAGS([-mtune=generic], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-msse], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-msse2], [], [-Werror])
    ],

    [i686], [
        AX_APPEND_COMPILE_FLAGS([-march=prescott], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-mtune=generic], [], [-Werror])
        AX_APPEND_COMPILE_FLAGS([-mcmov], [], [-Werror])
    ],

    [AC_MSG_WARN([unknown host_cpu])]
)

AX_APPEND_COMPILE_FLAGS([-pipe], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-mfpmath=sse], [], [-Werror])
AX_APPEND_COMPILE_FLAGS([-msahf], [], [-Werror])

###############################################################################
## Output

# inclusion of config.h has to go after compilation tests otherwise we risk
# failure on a clean build
AX_APPEND_FLAG([-include config.h])

AC_CONFIG_FILES([
    Doxyfile
    Makefile
    test/Makefile
])
AC_CONFIG_FILES([test/json.test], [chmod a+x test/json.test])
AC_OUTPUT
