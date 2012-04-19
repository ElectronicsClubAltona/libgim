AUTOMAKE_OPTIONS = dist-bzip2 dist-xz foreign subdir-objects
ACLOCAL_AMFLAGS = -I m4

AM_CXXFLAGS = -std=c++0x $(BOOST_CPPFLAGS) $(COMMON_CXXFLAGS)
#AM_LDFLAGS  = $(BOOST_LDFLAGS) $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB) $(COMMON_LDFLAGS)

SUBDIRS = test

UTIL_FILES   =		\
    adler.hpp		\
    adler.cpp		\
    annotations.hpp 	\
    backtrace.hpp   	\
    bitwise.cpp		\
    bitwise.hpp		\
    bsdsum.hpp		\
    bsdsum.cpp		\
    colour.cpp		\
    colour.hpp		\
    crc.cpp		\
    crc.hpp		\
    debug.cpp   	\
    debug.hpp   	\
    enable_if.hpp  	\
    endian.cpp		\
    endian.hpp		\
    except.cpp  	\
    except.hpp  	\
    extent.cpp		\
    extent.hpp		\
    fixed.cpp		\
    fixed.hpp		\
    fletcher.hpp	\
    float.cpp		\
    float.hpp		\
    fwd.hpp		\
    guid.cpp		\
    guid.hpp		\
    hash.cpp		\
    hash.hpp		\
    io.cpp		\
    io.hpp		\
    ip.cpp		\
    ip.hpp		\
    json.cpp		\
    json.hpp		\
    lerp.cpp		\
    lerp.hpp		\
    maths.cpp		\
    maths.hpp		\
    maths/matrix.cpp	\
    maths/matrix.hpp	\
    maths/vector.cpp  	\
    maths/vector.hpp  	\
    matrix.cpp		\
    matrix.hpp		\
    nocopy.hpp		\
    perlin.cpp		\
    perlin.hpp		\
    point.cpp		\
    point.hpp		\
    pool.cpp		\
    pool.hpp		\
    random.cpp		\
    random.hpp		\
    range.cpp		\
    range.hpp		\
    region.cpp		\
    region.hpp		\
    si.cpp		\
    signal.cpp		\
    signal.hpp		\
    si.hpp		\
    stream.cpp		\
    stream.hpp		\
    string.cpp		\
    string.hpp		\
    time.cpp		\
    time.hpp		\
    types.cpp		\
    types.hpp		\
    vector.cpp		\
    version.cpp		\
    version.hpp


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
libutil_la_SOURCES  = $(UTIL_FILES)
libutil_la_CXXFLAGS = $(AM_CXXFLAGS)
libutil_la_LIBADD   = $(BOOST_SYSTEM_LIB) $(BOOST_SYSTEM_LIB)

bin_PROGRAMS = 		\
    json-clean		\
    json-validate

json_clean_SOURCES    = json/clean.cpp
json_clean_DEPENDENCIES = $(top_builddir)/.libs/libutil.la
json_clean_LDFLAGS    = 		\
    $(BOOST_LDFLAGS)  			\
    $(BOOST_FILESYSTEM_LIB) 		\
    $(BOOST_SYSTEM_LIB)			\
    $(top_builddir)/.libs/libutil.a

json_schema_SOURCES = json/schema.cpp
json_schema_DEPENDENCIES = $(top_builddir)/.libs/libutil.la
json_schema_LDFLAGS    = 		\
    $(BOOST_LDFLAGS)  			\
    $(BOOST_FILESYSTEM_LIB) 		\
    $(BOOST_SYSTEM_LIB)			\
    $(top_builddir)/.libs/libutil.a


json_validate_SOURCES = json/validate.cpp
json_validate_DEPENDENCIES = $(top_builddir)/.libs/libutil.la
json_validate_LDFLAGS    = 		\
    $(BOOST_LDFLAGS)  			\
    $(BOOST_FILESYSTEM_LIB) 		\
    $(BOOST_SYSTEM_LIB)			\
    $(top_builddir)/.libs/libutil.a
