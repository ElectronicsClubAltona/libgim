AUTOMAKE_OPTIONS = dist-bzip2 dist-xz foreign
ACLOCAL_AMFLAGS = -I m4

AM_CXXFLAGS = $(BOOST_CFLAGS) $(BOOST_FILESYSTEM_CFLAGS) $(COMMON_CXXFLAGS)
AM_LDFLAGS  = $(BOOST_LIB) $(BOOST_FILESYSTEM_LIB) $(COMMON_LDFLAGS)

SUBDIRS = test

UTIL_INCLUDE =  \
    annotations.hpp \
    backtrace.hpp   \
    debug.hpp   \
    except.hpp  \
    float.hpp	\
    io.hpp	\
    json.hpp	\
    maths.hpp	\
    matrix.hpp	\
    nocopy.hpp	\
    range.hpp	\
    region.hpp	\
    stream.hpp	\
    types.hpp	\
    vector.hpp  \
    version.hpp

UTIL_FILES   =	\
    backtrace.cpp \
    debug.cpp   \
    except.cpp  \
    float.cpp	\
    io.cpp	\
    json.cpp	\
    maths.cpp	\
    matrix.cpp	\
    range.cpp	\
    region.cpp	\
    stream.cpp	\
    types.cpp	\
    vector.cpp  \
    version.cpp

CLEANFILES = json.cpp version.cpp
EXTRA_DIST = json.cpp.rl version.cpp.rl

RAGELFLAGS      = -F1
SUFFIXES = .cpp .cpp.rl
.cpp.rl.cpp:
	$(RAGEL) $(RAGELFLAGS) -C $< -o $(builddir)/$@

lib_LTLIBRARIES  = libutil.la
libutil_la_SOURCES  = $(UTIL_INCLUDE) $(UTIL_FILES)
libutil_la_CXXFLAGS = -fPIC $(COMMON_CXXFLAGS)
