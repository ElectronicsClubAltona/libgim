###############################################################################
## Global build rules

ACLOCAL_AMFLAGS = -I m4

AM_CXXFLAGS = $(BOOST_CPPFLAGS) $(ZLIB_CFLAGS)

###############################################################################
## Source definitions

UTIL_FILES   =		\
    aabb.cpp		\
    aabb.hpp		\
    backtrace.hpp   	\
    bezier.cpp		\
    bezier.hpp		\
    bitwise.cpp		\
    bitwise.hpp		\
    colour.cpp		\
    colour.hpp		\
    colour.ipp		\
    coord.hpp		\
    coord/base.hpp	\
    coord/init.hpp	\
    coord/names.hpp	\
    coord/ops.hpp	\
    coord/store.hpp	\
    crypto/tea.cpp	\
    crypto/tea.hpp	\
    crypto/xtea.cpp	\
    crypto/xtea.hpp	\
    crypto/xxtea.cpp	\
    crypto/xxtea.hpp	\
    crypto/arc4.cpp	\
    crypto/arc4.hpp	\
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
    hash/hmac.cpp	\
    hash/hmac.hpp	\
    hash/hotp.cpp	\
    hash/hotp.hpp	\
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
    introspection.cpp	\
    introspection.hpp	\
    io.cpp		\
    io.hpp		\
    io.ipp		\
    ip.cpp		\
    ip.hpp		\
    iterator.hpp	\
    json/fwd.hpp	\
    json/except.cpp	\
    json/except.hpp	\
    json/flat.cpp	\
    json/flat.hpp	\
    json/schema.cpp	\
    json/schema.hpp	\
    json/tree.cpp	\
    json/tree.hpp	\
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
    matrix.ipp		\
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
    plane.cpp		\
    plane.hpp		\
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
    range.ipp		\
    rational.cpp	\
    rational.hpp	\
    ray.cpp		\
    ray.hpp		\
    region.cpp		\
    region.hpp		\
    si.cpp		\
    signal.cpp		\
    signal.hpp		\
    signal.ipp		\
    si.hpp		\
    sphere.cpp		\
    sphere.hpp		\
    stats.cpp		\
    stats.hpp		\
    stream.cpp		\
    stream.hpp		\
    string.cpp		\
    string.hpp		\
    stringid.cpp	\
    stringid.hpp	\
    tap.cpp		\
    tap.hpp		\
    tap.ipp		\
    time.cpp		\
    time.hpp		\
    tuple.cpp		\
    tuple.hpp		\
    types.hpp		\
    types/bits.hpp	\
    types/casts.hpp	\
    types/comparator.hpp\
    types/comparator.ipp\
    types/string.cpp	\
    types/string.hpp	\
    types/traits.hpp	\
    uri.hpp		\
    uri.cpp		\
    variadic.cpp	\
    variadic.hpp	\
    variadic.ipp	\
    vector.cpp		\
    vector.hpp		\
    version.cpp		\
    version.hpp		\
    view.cpp		\
    view.hpp		\
    zlib.cpp		\
    zlib.hpp


if PLATFORM_LINUX
UTIL_FILES += 			\
    backtrace_execinfo.cpp	\
    io_posix.cpp		\
    io_posix.hpp
endif

if PLATFORM_WIN32
UTIL_FILES += 			\
    backtrace_null.cpp		\
    io_win32.cpp
endif

###############################################################################
## Local build rules
CLEANFILES = json.cpp version.cpp ip.cpp
EXTRA_DIST = json/flat.cpp.rl version.cpp.rl ip.cpp.rl uri.cpp.rl

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

bin_PROGRAMS = 				\
    tools/hash				\
    tools/json-clean			\
    tools/json-validate			\
    tools/json-schema

LDADD = $(top_builddir)/libutil.a $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB) $(ZLIB_LIBS)


##-----------------------------------------------------------------------------
## Tests

AM_LDFLAGS  += $(BOOST_LDFLAGS) $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB)
AM_CXXFLAGS += -I$(top_srcdir)

TEST_BIN = 			\
    test/aabb			\
    test/backtrace 		\
    test/bezier			\
    test/bitwise		\
    test/checksum		\
    test/colour			\
    test/crypto/arc4		\
    test/crypto/tea		\
    test/crypto/xtea		\
    test/crypto/xxtea		\
    test/extent			\
    test/fixed			\
    test/float			\
    test/hmac			\
    test/hotp			\
    test/hton			\
    test/introspection		\
    test/ip 			\
    test/json_types		\
    test/ray 			\
    test/maths			\
    test/maths_matrix 		\
    test/matrix			\
    test/md2			\
    test/md4			\
    test/md5			\
    test/options		\
    test/point			\
    test/polynomial		\
    test/pool			\
    test/rand			\
    test/range 			\
    test/region			\
    test/ripemd			\
    test/sha1			\
    test/sha2			\
    test/signal			\
    test/stringid		\
    test/tuple			\
    test/uri			\
    test/vector			\
    test/version


if PLATFORM_WIN32
test_hton_LDFLAGS = -lws2_32
endif

LOG_DRIVER = env AM_TAP_AWK='$(AWK)' $(SHELL) $(top_srcdir)/build-aux/tap-driver.sh
TESTS = $(top_builddir)/test/json-parse $(top_builddir)/test/json-schema $(TEST_BIN)
check_PROGRAMS = $(TEST_BIN)
EXTRA_DIST += test/json test/json-parse test/json-schema
