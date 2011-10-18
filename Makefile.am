AUTOMAKE_OPTIONS = dist-bzip2 dist-xz foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4

AM_CXXFLAGS = $(BOOST_CPPFLAGS) $(COMMON_CXXFLAGS)
#AM_LDFLAGS  = $(BOOST_LDFLAGS) $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB) $(COMMON_LDFLAGS)

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
    maths/matrix.hpp	\
    maths/vector.hpp  	\
    matrix.hpp		\
    nocopy.hpp		\
    point.hpp		\
    pool.hpp		\
    random.hpp		\
    range.hpp		\
    region.hpp		\
    signal.hpp		\
    si.hpp		\
    stream.hpp		\
    string.hpp		\
    time.hpp		\
    types.hpp		\
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
    maths/matrix.cpp	\
    maths/vector.cpp  	\
    matrix.cpp		\
    point.cpp		\
    pool.cpp		\
    random.cpp		\
    range.cpp		\
    region.cpp		\
    si.cpp		\
    signal.cpp		\
    stream.cpp		\
    string.cpp		\
    time.cpp		\
    types.cpp		\
    vector.cpp		\
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
libutil_la_CXXFLAGS = $(AM_CXXFLAGS)
libutil_la_LIBADD   = $(BOOST_SYSTEM_LIB)
