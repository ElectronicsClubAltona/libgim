AUTOMAKE_OPTIONS = dist-bzip2 dist-xz foreign
ACLOCAL_AMFLAGS = -I m4

AM_CXXFLAGS = $(BOOST_CPPFLAGS) $(COMMON_CXXFLAGS)
AM_LDFLAGS  = $(BOOST_LDFLAGS) $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB) $(COMMON_LDFLAGS)

SUBDIRS = test

UTIL_INCLUDE =  	\
    annotations.hpp 	\
    backtrace.hpp   	\
    bitwise.hpp		\
    colour.hpp		\
    debug.hpp   	\
    enable_if.hpp  	\
    endian.hpp		\
    except.hpp  	\
    fixed.hpp		\
    float.hpp		\
    fwd.hpp		\
    guid.hpp		\
    io.hpp		\
    ip.hpp		\
    json.hpp		\
    maths.hpp		\
    matrix.hpp		\
    nocopy.hpp		\
    point.hpp		\
    pool.hpp		\
    random.hpp		\
    range.hpp		\
    region.hpp		\
    signal.hpp		\
    stream.hpp		\
    string.hpp		\
    si.hpp		\
    time.hpp		\
    types.hpp		\
    vector.hpp  	\
    version.hpp

UTIL_FILES   =		\
    bitwise.cpp		\
    colour.cpp		\
    debug.cpp   	\
    endian.cpp		\
    except.cpp  	\
    fixed.cpp		\
    float.cpp		\
    guid.cpp		\
    io.cpp		\
    ip.cpp		\
    json.cpp		\
    maths.cpp		\
    matrix.cpp		\
    point.cpp		\
    pool.cpp		\
    random.cpp		\
    range.cpp		\
    region.cpp		\
    signal.cpp		\
    stream.cpp		\
    string.cpp		\
    si.cpp		\
    time.cpp		\
    types.cpp		\
    vector.cpp  	\
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
libutil_la_CXXFLAGS = -fPIC $(AM_CXXFLAGS) -lrt
libutil_la_LIBADD   = $(BOOST_SYSTEM_LIB)
