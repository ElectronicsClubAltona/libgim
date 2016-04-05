AC_INIT([util-cruft], [0.1.0], [danny@nerdcruft.net])
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

AC_LANG([C++])

NC_CXX
NC_PLATFORM
NC_OPTIMISATION
NC_WARNINGS
NC_DEBUGGING

LT_INIT

AM_INIT_AUTOMAKE([1.14 dist-bzip2 dist-xz foreign subdir-objects])
AM_SILENT_RULES([yes])
AM_MAINTAINER_MODE([enable])

AC_CONFIG_HEADERS([config.h])

AX_CHECK_GNU_MAKE

###############################################################################
## Architecture features

AC_C_BIGENDIAN

###############################################################################
## Useful headers or platform features

###############################################################################
## Platform features

AC_DEFINE([_XOPEN_SOURCE], [700], [use POSIX 2008])
AC_DEFINE([__STDC_FORMAT_MACROS], [1], [use C format macros])

AC_FUNC_MMAP

AC_CHECK_FUNC([backtrace])
AM_CONDITIONAL([HAVE_EXECINFO], [test "x$ac_cv_func_backtrace" = "xtrue"])

AC_SEARCH_LIBS([clock_gettime], [rt], [], [
    # windows doesn't have clock_gettime
    AS_IF([test "x${host_os}" != "xmingw32"], [
        AC_MSG_ERROR([unable to find the clock library])
    ])
])

AC_SEARCH_LIBS([cos], [m], [], [AC_MSG_ERROR([unable to find the maths library])])

## Find some POSIX functions that windows doesn't support
AS_IF([test "x${host_os}" != "xmingw32"],[
    AC_SEARCH_LIBS([dlopen], [dl])
    AC_SEARCH_LIBS([shm_open], [rt], [], [AC_MSG_ERROR([unable to find shm library])])
])

AC_SEARCH_LIBS([htons], [ws2_32], [], [AC_MSG_ERROR([unable to find htnos library])])


###############################################################################
## Debug features

###############################################################################
## Documentation

###############################################################################
## Required packages

CHECK_RAGEL([ip.cpp])

NC_BOOST([1.53], [system filesystem thread])
AC_SUBST(BOOST_CPPFLAGS)
AC_SUBST(BOOST_LDFLAGS)

###############################################################################
## Optional packages

AC_CHECK_TOOL([ADDR2LINE], [addr2line], [:])
AS_IF([test "x$ADDR2LINE" != "x:"], [
    AC_DEFINE_UNQUOTED([ADDR2LINE], ["$ADDR2LINE"], [addr2line tool name])
])

###############################################################################
## Performance and build optimisations

###############################################################################
## Output

AC_SUBST(LIBS)

# inclusion of config.h has to go after compilation tests otherwise we risk
# failure on a clean build
AX_APPEND_FLAG([-include config.h])

AC_CONFIG_FILES([
    Doxyfile
    Makefile
    libcruft-util.pc
])

AC_CONFIG_FILES([test/json-parse],  [chmod a+x test/json-parse])
AC_CONFIG_FILES([test/json-schema], [chmod a+x test/json-schema])

AC_OUTPUT
