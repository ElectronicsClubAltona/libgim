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

AC_LANG([C++])

NC_CXX
NC_PLATFORM
NC_OPTIMISATION
NC_WARNINGS
NC_DEBUGGING

LT_INIT

AM_INIT_AUTOMAKE([1.14 dist-bzip2 dist-xz foreign subdir-objects])
AM_SILENT_RULES([yes])

AC_CONFIG_HEADERS([config.h])

AX_CHECK_GNU_MAKE

###############################################################################
## Architecture features

AC_C_BIGENDIAN

###############################################################################
## Useful headers or platform features

AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

###############################################################################
## Platform features

AC_DEFINE([_XOPEN_SOURCE], [700], [use POSIX 2008])

AC_FUNC_MMAP

AC_CHECK_HEADERS([execinfo.h], [break;], [have_execinfo_h=true])
AM_CONDITIONAL([HAVE_EXECINFO_H], [test "x$have_execinfo_h" = "xtrue"])


AC_SEARCH_LIBS([clock_gettime], [rt])
AS_IF([test "x$ac_cv_search_clock_gettime" == "x-*"], [
    AX_APPEND_LINK_FLAGS([$ac_cv_search_clock_gettime], [], [-Werror])
])

## Use dynamic loader if present (for util::library)
AS_IF(
    [test "x${host_os}" != "xmingw32"],
    [AC_SEARCH_LIBS([dlopen], [dl])]
)


###############################################################################
## Debug features

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

AC_CHECK_TOOL([ADDR2LINE], [addr2line], [:])
AS_IF([test "x$ADDR2LINE" != "x:"], [AC_DEFINE_UNQUOTED([ADDR2LINE], ["$ADDR2LINE"], [addr2line tool name])])

###############################################################################
## Performance and build optimisations

###############################################################################
## Output

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
