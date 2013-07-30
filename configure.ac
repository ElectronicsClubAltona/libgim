AC_INIT([libgim], [0.1.0], [danny@nerdcruft.net])
## Explicitly set an empty CXXFLAGS if not present to prevent AC_PROG_CXX from
## generating a default -O2. This allows us to manually select -O0 when
## debugging is enabled.
: ${CXXFLAGS=""}

###############################################################################
## Build environment discovery

AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AC_USE_SYSTEM_EXTENSIONS
AC_CANONICAL_HOST

AC_LANG([C++])
AC_PROG_CXX
AC_PROG_CXXCPP

AX_CXX_COMPILE_STDCXX_11([noext])

LT_INIT

AM_INIT_AUTOMAKE([1.14 dist-bzip2 dist-xz foreign subdir-objects])
AM_SILENT_RULES([yes])

AC_CONFIG_HEADERS([config.h])

###############################################################################
## Major compilation options

AC_ARG_ENABLE([debugging],
    [AS_HELP_STRING([--enable-debugging], [enables developer debugging support])]
)

AC_ARG_ENABLE([mudflap],
    [AS_HELP_STRING([--enable-mudflap], [enable mudflap runtime debugging])]
)

###############################################################################
## Warnings

AX_APPEND_COMPILE_FLAGS([-Wall])
AX_APPEND_COMPILE_FLAGS([-Wextra])
AX_APPEND_COMPILE_FLAGS([-Wno-parentheses])
AX_APPEND_COMPILE_FLAGS([-Wpointer-arith])
AX_APPEND_COMPILE_FLAGS([-Wcast-qual])
AX_APPEND_COMPILE_FLAGS([-Wcast-align])
AX_APPEND_COMPILE_FLAGS([-Wsign-compare])
AX_APPEND_COMPILE_FLAGS([-Wsign-conversion])
AX_APPEND_COMPILE_FLAGS([-Wtype-limits])
AX_APPEND_COMPILE_FLAGS([-Wfloat-equal])
AX_APPEND_COMPILE_FLAGS([-Wunused-parameter])
AX_APPEND_COMPILE_FLAGS([-Wunused-but-set-variable])
AX_APPEND_COMPILE_FLAGS([-Wshadwo])
AX_APPEND_COMPILE_FLAGS([-Wredundant-decls])

AX_APPEND_COMPILE_FLAGS([-pedantic])

######################################################################
## Platform compilation helpers

AX_APPEND_COMPILE_FLAGS([-fno-common])
AX_APPEND_COMPILE_FLAGS([-fno-nonansi-builtins])
AX_APPEND_COMPILE_FLAGS([-fno-deduce-init-list])

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
    AX_APPEND_LINK_FLAGS([$ac_cv_search_clock_gettime])
])

###############################################################################
## Debug features

AS_IF([test "x$enable_debugging" = "xyes"], [
    AS_IF([test "x$enable_mudflap" = "xyes"], [
        AX_APPEND_COMPILE_FLAGS([-fmudflapth])
        AX_APPEND_LINK_FLAGS([-lmudflapth])
    ])

    AX_APPEND_COMPILE_FLAGS([-ggdb])

    AC_DEFINE([ENABLE_DEBUGGING], [], [Debugging support enabled])
    AC_DEFINE([_GLIBCXX_DEBUG],   [], [Use glibcxx debugging mode])

    AX_APPEND_COMPILE_FLAGS([-O0])
], [
    AX_APPEND_COMPILE_FLAGS([-02])
    AX_APPEND_COMPILE_FLAGS([-flto=8])
    AX_APPEND_COMPILE_FLAGS([-flto-partition=none])

    AX_APPEND_LINK_FLAGS([-flto=8])
    AX_APPEND_LINK_FLAGS([-flto-partition=none])
    AX_APPEND_LINK_FLAGS([-fwhole-program])
])

AS_CASE([$host_os],
    [mingw32], [
        AX_APPEND_COMPILE_FLAGS([-gstabs])

        ## HACK: Stop multiple defines from boost exception rethrowers
        ## terminating the build. Works around boost bug #4258.
        AX_APPEND_COMPILE_FLAGS([-Wl,--allow-multiple-definition])
    ],

    [linux-gnu], [
        AX_APPEND_COMPILE_FLAGS([-g])
        AX_APPEND_COMPILE_FLAGS([-ggdb])
    ],

    [AC_MSG_WARN([Unknown host_os])]
)

###############################################################################
## Documentation

###############################################################################
## Required packages

CHECK_RAGEL([ip.cpp])

AX_BOOST_BASE([1.53], [], [AC_MSG_ERROR([Boost version >= 1.53 required])])
AC_SUBST(BOOST_CPPFLAGS)
AC_SUBST(BOOST_LDFLAGS)

# boost-system isn't a hard requirement, it's only really used to fulfill
# some other dependency I've since forgotten about...
AX_BOOST_SYSTEM

AX_BOOST_FILESYSTEM
AC_SUBST(BOOST_FILESYSTEM_LIB)

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
        AX_APPEND_COMPILE_FLAGS([-mtune=generic])
    ],

    [i686], [
        AX_APPEND_COMPILE_FLAGS([-march=prescott])
        AX_APPEND_COMPILE_FLAGS([-mtune=generic])
        AX_APPEND_COMPILE_FLAGS([-mfpmath=sse])
    ],

    [AC_MSG_WARN([unknown host_cpu])]
)

AX_APPEND_COMPILE_FLAGS([-pipe])

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
AC_OUTPUT
