###############################################################################
## Global build rules

ACLOCAL_AMFLAGS = -I m4

AM_CXXFLAGS = $(BOOST_CPPFLAGS) $(ZLIB_CFLAGS)

SUBDIRS = test

###############################################################################
## Source definitions

UTIL_FILES   =		\
    backtrace.hpp   	\
    bezier.cpp		\
    bezier.hpp		\
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
    extent.ipp		\
    fixed.cpp		\
    fixed.hpp		\
    float.cpp		\
    float.hpp		\
    fourcc.cpp		\
    fourcc.hpp		\
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
    hash/fletcher.hpp	\
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
    io.ipp		\
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
    maths.ipp		\
    maths/matrix.cpp	\
    maths/matrix.hpp	\
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
    pascal.cpp		\
    pascal.hpp		\
    platform.hpp	\
    point.cpp		\
    point.hpp		\
    point.ipp		\
    polynomial.cpp	\
    polynomial.hpp	\
    polynomial.ipp	\
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
    rational.cpp	\
    rational.hpp	\
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
    stringid.cpp	\
    stringid.hpp	\
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
    vector.ipp		\
    version.cpp		\
    version.hpp		\
    zlib.cpp		\
    zlib.hpp


if PLATFORM_LINUX
UTIL_FILES += 			\
    backtrace_execinfo.cpp	\
    io_posix.cpp
endif

if PLATFORM_WIN32
UTIL_FILES += 			\
    backtrace_null.cpp		\
    io_win32.cpp
endif

###############################################################################
## Local build rules
CLEANFILES = json.cpp version.cpp ip.cpp
EXTRA_DIST = json.cpp.rl version.cpp.rl ip.cpp.rl

RAGELFLAGS      = -F1
SUFFIXES = .cpp .cpp.rl
.cpp.rl.cpp:
	$(RAGEL) $(RAGELFLAGS) -C $< -o $(builddir)/$@

###############################################################################
## Library definition
lib_LIBRARIES = libutil.a
libutil_a_SOURCES  = $(UTIL_FILES)
libutil_a_CXXFLAGS = $(AM_CXXFLAGS)

###############################################################################
## Utility programs

AM_DEFAULT_SOURCE_EXT = .cpp

AM_LDFLAGS  = $(BOOST_LDFLAGS)
LDADD = $(top_builddir)/libutil.a $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB) $(ZLIB_LIBS)

bin_PROGRAMS = 				\
    tools/hash				\
    tools/json-clean			\
    tools/json-validate			\
    tools/json-schema

tools_hash_LDADD = $(top_builddir)/libutil.a $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB) $(ZLIB_LIBS)
