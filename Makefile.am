ACLOCAL_AMFLAGS = -I m4

AM_CXXFLAGS = $(BOOST_CPPFLAGS) $(COMMON_CXXFLAGS) $(ZLIB_CFLAGS)
AM_LDFLAGS  = $(BOOST_LDFLAGS) $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB) $(COMMON_LDFLAGS) $(ZLIB_LIBS)

SUBDIRS = test

UTIL_FILES   =		\
    annotations.hpp 	\
    backtrace.hpp   	\
    bitwise.cpp		\
    bitwise.hpp		\
    colour.cpp		\
    colour.hpp		\
    detail/coord.hpp	\
    debug.cpp   	\
    debug.hpp   	\
    endian.cpp		\
    endian.hpp		\
    except.cpp  	\
    except.hpp  	\
    exe.cpp		\
    exe.hpp		\
    extent.cpp		\
    extent.hpp		\
    fixed.cpp		\
    fixed.hpp		\
    fletcher.hpp	\
    float.cpp		\
    float.hpp		\
    fourcc.cpp		\
    fourcc.hpp		\
    fwd.hpp		\
    guid.cpp		\
    guid.hpp		\
    hash.cpp		\
    hash.hpp		\
    hash/adler.cpp	\
    hash/adler.hpp	\
    hash/bsdsum.hpp	\
    hash/bsdsum.cpp	\
    hash/crc.cpp	\
    hash/crc.hpp	\
    hash/md2.cpp	\
    hash/md2.hpp	\
    hash/md4.cpp	\
    hash/md4.hpp	\
    hash/md5.cpp	\
    hash/md5.hpp	\
    hash/pbkdf1.cpp	\
    hash/pbkdf1.hpp	\
    hash/pbkdf2.cpp	\
    hash/pbkdf2.hpp	\
    hash/ripemd.cpp	\
    hash/ripemd.hpp	\
    hash/sha2.cpp	\
    hash/sha2.hpp	\
    hash/sha1.cpp	\
    hash/sha1.hpp	\
    image.cpp		\
    image.hpp		\
    io.cpp		\
    io.hpp		\
    ip.cpp		\
    ip.hpp		\
    iterator.hpp	\
    json.cpp		\
    json.hpp		\
    lerp.cpp		\
    lerp.hpp		\
    log.cpp		\
    log.hpp		\
    log.ipp		\
    maths.cpp		\
    maths.hpp		\
    maths/matrix.cpp	\
    maths/matrix.hpp	\
    maths/polynomial.ipp\
    maths/polynomial.hpp\
    maths/vector.cpp  	\
    maths/vector.hpp  	\
    matrix.cpp		\
    matrix.hpp		\
    memory.cpp		\
    memory.hpp		\
    net/address.cpp	\
    net/address.hpp	\
    net/except.cpp	\
    net/except.hpp	\
    net/types.cpp	\
    net/types.hpp	\
    net/socket.cpp	\
    net/socket.hpp	\
    nocopy.hpp		\
    noise.cpp		\
    noise.hpp		\
    noise/basis.cpp	\
    noise/basis.hpp	\
    noise/fractal.cpp	\
    noise/fractal.hpp	\
    noise/lut.cpp	\
    noise/lut.hpp	\
    options.cpp		\
    options.hpp		\
    platform.hpp	\
    point.cpp		\
    point.hpp		\
    point.ipp		\
    pool.cpp		\
    pool.hpp		\
    pool.ipp		\
    preprocessor.hpp	\
    quaternion.cpp	\
    quaternion.hpp	\
    raii.hpp		\
    random.cpp		\
    random.hpp		\
    random.ipp		\
    range.cpp		\
    range.hpp		\
    region.cpp		\
    region.hpp		\
    si.cpp		\
    signal.cpp		\
    signal.hpp		\
    signal.ipp		\
    si.hpp		\
    stats.cpp		\
    stats.hpp		\
    stream.cpp		\
    stream.hpp		\
    string.cpp		\
    string.hpp		\
    time.cpp		\
    time.hpp		\
    types.hpp		\
    types/bits.hpp	\
    types/casts.hpp	\
    types/comparator.hpp\
    types/comparator.ipp\
    types/string.cpp	\
    types/string.hpp	\
    types/traits.hpp	\
    vector.cpp		\
    vector.hpp		\
    version.cpp		\
    version.hpp		\
    zlib.cpp		\
    zlib.hpp


if PLATFORM_LINUX
UTIL_FILES += backtrace_execinfo.cpp
endif

if PLATFORM_WIN32
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
libutil_la_LIBADD   = $(BOOST_SYSTEM_LIB)

###############################################################################
## Utility programs

LDADD = $(top_builddir)/.libs/libutil.a
bin_PROGRAMS = 				\
    tools/json-clean			\
    tools/json-validate			\
    tools/json-schema

tools_json_clean_SOURCES    	= tools/json-clean.cpp
tools_json_clean_DEPENDENCIES 	= $(top_builddir)/libutil.la
tools_json_clean_LDFLAGS    	= $(AM_LDFLAGS)

tools_json_schema_SOURCES 	= tools/json-schema.cpp
tools_json_schema_DEPENDENCIES 	= $(top_builddir)/libutil.la
tools_json_schema_LDFLAGS    	= $(AM_LDFLAGS)

tools_json_validate_SOURCES 		= tools/json-validate.cpp
tools_json_validate_DEPENDENCIES 	= $(top_builddir)/libutil.la
tools_json_validate_LDFLAGS    		= $(AM_LDFLAGS)
