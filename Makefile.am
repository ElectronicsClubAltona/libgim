###############################################################################
## Global build rules

ACLOCAL_AMFLAGS = -I m4

AM_CXXFLAGS = $(BOOST_CPPFLAGS) $(ZLIB_CFLAGS)

###############################################################################
## Source definitions

UTIL_FILES   =			\
    adapter.hpp			\
    adapter.cpp			\
    alloc/affix.cpp		\
    alloc/affix.hpp		\
    alloc/allocator.cpp		\
    alloc/allocator.hpp		\
    alloc/allocator.ipp		\
    alloc/arena.cpp		\
    alloc/arena.hpp		\
    alloc/arena.ipp		\
    alloc/fallback.cpp		\
    alloc/fallback.hpp		\
    alloc/linear.cpp		\
    alloc/linear.hpp		\
    alloc/malloc.cpp		\
    alloc/malloc.hpp		\
    alloc/null.cpp		\
    alloc/null.hpp		\
    alloc/stack.cpp		\
    alloc/stack.hpp		\
    backtrace.hpp   		\
    bezier.cpp			\
    bezier.hpp			\
    bitwise.cpp			\
    bitwise.hpp			\
    cast.hpp			\
    cmdopt.cpp			\
    cmdopt.hpp			\
    cmdopt.ipp			\
    colour.cpp			\
    colour.hpp			\
    colour.ipp			\
    coord/base.hpp		\
    coord.hpp			\
    coord/init.hpp		\
    coord/names.hpp		\
    coord/ops.hpp		\
    coord/store.hpp		\
    crypto/arc4.cpp		\
    crypto/arc4.hpp		\
    crypto/tea.cpp		\
    crypto/tea.hpp		\
    crypto/xtea.cpp		\
    crypto/xtea.hpp		\
    crypto/xxtea.cpp		\
    crypto/xxtea.hpp		\
    debug.cpp   		\
    debug.hpp   		\
    debug.ipp   		\
    endian.cpp			\
    endian.hpp			\
    except.cpp  		\
    except.hpp  		\
    exe.cpp			\
    exe.hpp			\
    extent.cpp			\
    extent.hpp			\
    extent.ipp			\
    fixed.cpp			\
    fixed.hpp			\
    float.cpp			\
    float.hpp			\
    format.cpp			\
    format.hpp			\
    format.ipp			\
    fourcc.cpp			\
    fourcc.hpp			\
    geom/fwd.hpp		\
    geom/aabb.cpp		\
    geom/aabb.hpp		\
    geom/aabb.ipp		\
    geom/cylinder.cpp		\
    geom/cylinder.hpp		\
    geom/ellipse.cpp		\
    geom/ellipse.hpp		\
    geom/ellipse.ipp		\
    geom/iostream.cpp		\
    geom/iostream.hpp		\
    geom/ops.hpp		\
    geom/plane.cpp		\
    geom/plane.hpp		\
    geom/ray.cpp		\
    geom/ray.hpp		\
    geom/rect.cpp		\
    geom/rect.hpp		\
    geom/sample.hpp		\
    geom/sample.ipp		\
    geom/sphere.cpp		\
    geom/sphere.hpp		\
    geom/tri.cpp		\
    geom/tri.hpp		\
    guid.cpp			\
    guid.hpp			\
    hash.hpp			\
    hash/adler.cpp		\
    hash/adler.hpp		\
    hash/bsdsum.cpp		\
    hash/bsdsum.hpp		\
    hash/crc.cpp		\
    hash/crc.hpp		\
    hash/fasthash.cpp		\
    hash/fasthash.hpp		\
    hash/fletcher.hpp		\
    hash/fnv1a.cpp		\
    hash/fnv1a.hpp		\
    hash/hmac.cpp		\
    hash/hmac.hpp		\
    hash/hotp.cpp		\
    hash/hotp.hpp		\
    hash/md2.cpp		\
    hash/md2.hpp		\
    hash/md4.cpp		\
    hash/md4.hpp		\
    hash/md5.cpp		\
    hash/md5.hpp		\
    hash/murmur/common.cpp	\
    hash/murmur/common.hpp	\
    hash/murmur.hpp		\
    hash/murmur/murmur1.cpp	\
    hash/murmur/murmur1.hpp	\
    hash/murmur/murmur2.cpp	\
    hash/murmur/murmur2.hpp	\
    hash/murmur/murmur3.cpp	\
    hash/murmur/murmur3.hpp	\
    hash/pbkdf1.cpp		\
    hash/pbkdf1.hpp		\
    hash/pbkdf2.cpp		\
    hash/pbkdf2.hpp		\
    hash/ripemd.cpp		\
    hash/ripemd.hpp		\
    hash/sha1.cpp		\
    hash/sha1.hpp		\
    hash/sha2.cpp		\
    hash/sha2.hpp		\
    hash/wang.hpp		\
    hash/wang.ipp		\
    image.cpp			\
    image.hpp			\
    image.ipp			\
    introspection.cpp		\
    introspection.hpp		\
    io.cpp			\
    io.hpp			\
    io.ipp			\
    ip.cpp			\
    ip.hpp			\
    iterator.hpp		\
    json/fwd.hpp		\
    json/except.cpp		\
    json/except.hpp		\
    json/flat.cpp		\
    json/flat.hpp		\
    json/schema.cpp		\
    json/schema.hpp		\
    json/tree.cpp		\
    json/tree.hpp		\
    library.hpp			\
    log.cpp			\
    log.hpp			\
    log.ipp			\
    maths.cpp			\
    maths.hpp			\
    matrix.cpp			\
    matrix2.cpp			\
    matrix3.cpp			\
    matrix4.cpp			\
    matrix.hpp			\
    matrix.ipp			\
    memory/buffer/circular.cpp	\
    memory/buffer/circular.hpp	\
    memory/buffer/paged.cpp	\
    memory/buffer/paged.hpp	\
    memory/deleter.cpp		\
    memory/deleter.hpp		\
    memory/system.cpp		\
    memory/system.hpp		\
    net/address.cpp		\
    net/address.hpp		\
    net/except.cpp		\
    net/except.hpp		\
    net/socket.cpp		\
    net/socket.hpp		\
    net/types.cpp		\
    net/types.hpp		\
    netpbm.cpp			\
    netpbm.hpp			\
    nocopy.hpp			\
    noise.hpp			\
    noise.ipp			\
    noise/fwd.hpp		\
    noise/basis/constant.cpp	\
    noise/basis/constant.hpp	\
    noise/basis/constant.ipp	\
    noise/basis/gradient/uniform.hpp	\
    noise/basis/gradient/uniform.ipp	\
    noise/basis/gradient/exp.hpp	\
    noise/basis/gradient/exp.ipp	\
    noise/basis/patch.hpp	\
    noise/basis/patch.ipp	\
    noise/basis/perlin.hpp	\
    noise/basis/perlin.ipp	\
    noise/basis/runtime.cpp	\
    noise/basis/runtime.hpp	\
    noise/basis/type/distance.cpp	\
    noise/basis/type/distance.hpp	\
    noise/basis/type/gradient.hpp	\
    noise/basis/type/gradient.cpp	\
    noise/basis/value.hpp	\
    noise/basis/value.ipp	\
    noise/basis/worley.hpp	\
    noise/basis/worley.ipp	\
    noise/fractal/base.hpp	\
    noise/fractal/base.ipp	\
    noise/fractal/fbm.hpp	\
    noise/fractal/fbm.ipp	\
    noise/fractal/hetero.hpp	\
    noise/fractal/hetero.ipp	\
    noise/fractal/hmf.hpp	\
    noise/fractal/hmf.ipp	\
    noise/fractal/rmf.hpp	\
    noise/fractal/rmf.ipp	\
    noise/fractal/runtime.cpp	\
    noise/fractal/runtime.hpp	\
    noise/lerp.cpp		\
    noise/lerp.hpp		\
    noise/lut.cpp		\
    noise/lut.hpp		\
    noise/midpoint.cpp		\
    noise/midpoint.hpp		\
    noise/rand.hpp		\
    noise/rand/hash.hpp		\
    noise/rand/hash.ipp		\
    noise/rand/permute.cpp	\
    noise/rand/permute.hpp	\
    noise/rand/permute.ipp	\
    noise/turbulence.hpp	\
    noise/turbulence.ipp	\
    pascal.cpp			\
    pascal.hpp			\
    platform.hpp		\
    point.cpp			\
    point.hpp			\
    point.ipp			\
    pointer.hpp			\
    polynomial.cpp		\
    polynomial.hpp		\
    polynomial.ipp		\
    pool.cpp			\
    pool.hpp			\
    pool.ipp			\
    preprocessor.hpp		\
    quaternion.cpp		\
    quaternion.hpp		\
    raii.hpp			\
    rand/lcg.cpp		\
    rand/lcg.hpp		\
    rand/xorshift.cpp		\
    rand/xorshift.hpp		\
    random.cpp			\
    random.hpp			\
    random.ipp			\
    range.cpp			\
    range.hpp			\
    range.ipp			\
    rational.cpp		\
    rational.hpp		\
    rational.ipp		\
    region.cpp			\
    region.hpp			\
    region.ipp			\
    roots/bisection.hpp		\
    si.cpp			\
    signal.cpp			\
    signal.hpp			\
    signal.ipp			\
    si.hpp			\
    stats.cpp			\
    stats.hpp			\
    stream.cpp			\
    stream.hpp			\
    string.cpp			\
    string.hpp			\
    stringid.cpp		\
    stringid.hpp		\
    strongdef.cpp		\
    strongdef.hpp		\
    tap.cpp			\
    tap.hpp			\
    tap.ipp			\
    time.cpp			\
    time.hpp			\
    tuple.cpp			\
    tuple.hpp			\
    types/bits.hpp		\
    types/comparator.hpp	\
    types/comparator.ipp	\
    types.hpp			\
    types/string.cpp		\
    types/string.hpp		\
    types/traits.hpp		\
    uri.cpp			\
    uri.hpp			\
    variadic.cpp		\
    variadic.hpp		\
    variadic.ipp		\
    vector.cpp			\
    vector.hpp			\
    vector.ipp			\
    version.cpp			\
    version.hpp			\
    view.ipp			\
    view.hpp

if PLATFORM_LINUX
UTIL_FILES += 			\
    backtrace_execinfo.cpp	\
    io_posix.cpp		\
    io_posix.hpp		\
    library_posix.hpp		\
    library_posix.cpp		\
    posix/dir.hpp		\
    posix/dir.cpp		\
    time_posix.cpp

endif

if PLATFORM_WIN32
UTIL_FILES += 			\
    backtrace_null.cpp		\
    io_win32.cpp		\
    library_win32.hpp		\
    library_win32.cpp		\
    time_win32.cpp		\
    win32/registry.hpp		\
    win32/registry.cpp
endif

###############################################################################
## Local build rules
CLEANFILES = json.cpp version.cpp ip.cpp uri.cpp
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

AM_LDFLAGS  = $(BOOST_LDFLAGS) -lrt

bin_PROGRAMS = 				\
    tools/json-clean			\
    tools/json-validate			\
    tools/json-schema			\
    tools/hash				\
    tools/noise

noinst_PROGRAMS = tools/scratch

LDADD = $(top_builddir)/libutil.a $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB) $(ZLIB_LIBS)


##-----------------------------------------------------------------------------
## Tests

AM_LDFLAGS  += $(BOOST_LDFLAGS) $(BOOST_FILESYSTEM_LIB) $(BOOST_SYSTEM_LIB)
AM_CXXFLAGS += -I$(top_srcdir)

TEST_BIN = 			\
    test/alloc/arena		\
    test/alloc/stack		\
    test/alloc/linear		\
    test/backtrace 		\
    test/bezier			\
    test/bitwise		\
    test/checksum		\
    test/cmdopt			\
    test/colour			\
    test/coord			\
    test/crypto/arc4		\
    test/crypto/tea		\
    test/crypto/xtea		\
    test/crypto/xxtea		\
    test/extent			\
    test/fixed			\
    test/float			\
    test/format			\
    test/geom/aabb		\
    test/geom/ray 		\
    test/hash/murmur		\
    test/hash/fasthash          \
    test/hmac			\
    test/hotp			\
    test/hton			\
    test/image			\
    test/introspection		\
    test/ip 			\
    test/json_types		\
    test/memory/buffer/circular \
    test/memory/buffer/paged	\
    test/memory/deleter		\
    test/maths			\
    test/matrix			\
    test/md2			\
    test/md4			\
    test/md5			\
    test/point			\
    test/polynomial		\
    test/pool			\
    test/rand/buckets		\
    test/random			\
    test/range 			\
    test/rational		\
    test/region			\
    test/ripemd			\
    test/roots/bisection	\
    test/sha1			\
    test/sha2			\
    test/signal			\
    test/stringid		\
    test/strongdef		\
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
