AUTOMAKE_OPTIONS = dist-bzip2 dist-xz foreign
ACLOCAL_AMFLAGS = -I m4

AM_CXXFLAGS = $(BOOST_CPPFLAGS) $(COMMON_CXXFLAGS)
AM_LDFLAGS  = $(BOOST_LDFLAGS) $(BOOST_FILESYSTEM_LIB) $(COMMON_LDFLAGS)

SUBDIRS = test

UTIL_INCLUDE =  \
    annotations.hpp \
    backtrace.hpp   \
    debug.hpp   \
    enable_if.hpp   \
    except.hpp  \
    float.hpp	\
    io.hpp	\
    ip.hpp	\
    json.hpp	\
    maths.hpp	\
    matrix.hpp	\
    nocopy.hpp	\
    point.hpp	\
    pool.hpp	\
    range.hpp	\
    region.hpp	\
    signal.hpp	\
    stream.hpp	\
    types.hpp	\
    vector.hpp  \
    version.hpp

UTIL_FILES   =	\
    debug.cpp   \
    except.cpp  \
    float.cpp	\
    io.cpp	\
    ip.cpp	\
    json.cpp	\
    maths.cpp	\
    matrix.cpp	\
    point.cpp	\
    pool.cpp	\
    range.cpp	\
    region.cpp	\
    signal.cpp	\
    stream.cpp	\
    types.cpp	\
    vector.cpp  \
    version.cpp


if HAVE_EXECINFO
UTIL_FILES += backtrace_execinfo.cpp
else
UTIL_FILES += backtrace_null.cpp
endif

CLEANFILES = json.cpp version.cpp ip.cpp
EXTRA_DIST = json.cpp.rl version.cpp.rl ip.cpp.rl

RAGELFLAGS      = -F1
SUFFIXES = .cpp .cpp.rl
.cpp.rl.cpp:
	$(RAGEL) $(RAGELFLAGS) -C $< -o $(builddir)/$@

lib_LTLIBRARIES  = libutil.la
libutil_la_SOURCES  = $(UTIL_INCLUDE) $(UTIL_FILES)
libutil_la_CXXFLAGS = -fPIC $(AM_CXXFLAGS)
