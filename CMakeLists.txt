cmake_minimum_required(VERSION 3.7.0)
project(util-cruft)

set(CMAKE_MODULE_PATH
    ${CMAKE_MODULE_PATH}
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/cmake)

include(CheckFunctionExists)
include(CheckCXXCompilerFlag)

include (nc)

include (search_libs)

find_package(RAGEL 6.9 REQUIRED)


###############################################################################
include (TestBigEndian)
TEST_BIG_ENDIAN(ENDIANNESS)
if (ENDIANNESS)
    add_definitions(-DWORDS_BIGENDIAN)
endif()


###############################################################################
RAGEL_TARGET(json-flat json/flat.cpp.rl ${CMAKE_CURRENT_BINARY_DIR}/json/flat.cpp)
RAGEL_TARGET(uri uri.cpp.rl ${CMAKE_CURRENT_BINARY_DIR}/uri.cpp)
RAGEL_TARGET(version version.cpp.rl ${CMAKE_CURRENT_BINARY_DIR}/version.cpp)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})


###############################################################################
set (UTIL_FILES)


###############################################################################
search_libs (BACKTRACE_LIB backtrace execinfo)
search_libs (DBGHELP_LIB SymFromAddr dbghelp)
check_function_exists (RtlCaptureStackBackTrace HAVE_CAPTURESTACKBACKTRACE)


##-----------------------------------------------------------------------------
if (BACKTRACE_LIB_FOUND)
    list (APPEND UTIL_FILES backtrace_execinfo.cpp)
elseif (HAVE_CAPTURESTACKBACKTRACE)
    list (APPEND UTIL_FILES backtrace_stackwalk.cpp)
else ()
    list (APPEND UTIL_FILES backtrace_null.cpp)
endif ()


##-----------------------------------------------------------------------------
if (NOT WIN32)
    find_program(ADDR2LINE NAMES addr2line)
    if (ADDR2LINE)
        add_definitions(-DADDR2LINE="${ADDR2LINE}")
    endif()
endif ()


###############################################################################
# Platform wrappers
if (LINUX)
    list (APPEND UTIL_FILES exe_linux.cpp)
elseif (FREEBSD)
    list (APPEND UTIL_FILES exe_freebsd.cpp)
elseif (WIN32)
    list (APPEND UTIL_FILES exe_win32.cpp)
else ()
    message (FATAL_ERROR "unhandled platform")
endif ()


###############################################################################
# platform libraries
search_libs (SHM_LIBS shm_open rt)
search_libs (DL_LIBS dlopen dl)
search_libs (CLOCK_LIB clock_gettime rt c)
search_libs (MATH_LIB cos m)

list (APPEND LIBS ${SHM_LIBS} ${DL_LIBS} ${CLOCK_LIB} ${MATH_LIB})


###############################################################################
# platform wrappers
list (
    APPEND UTIL_FILES
    posix/dir.cpp
    posix/dir.hpp
    posix/dir.ipp
    posix/fd.cpp
    posix/fd.hpp
)


if (NOT WINDOWS)
    list (
        APPEND UTIL_FILES
        memory/buffer/circular.cpp
        memory/buffer/circular.hpp
        memory/buffer/paged.cpp
        memory/buffer/paged.hpp
        memory/system.cpp
        memory/system.hpp
        debug_posix.cpp
        io_posix.cpp
        io_posix.hpp
        io_posix.ipp
        library_posix.hpp
        library_posix.cpp
        posix/fwd.hpp
        posix/map.cpp
        posix/map.hpp
        time_posix.cpp
    )
endif ()


##-----------------------------------------------------------------------------
if (WINDOWS)
    list (
        APPEND UTIL_FILES
        debug_win32.cpp
        exe_win32.cpp
        io_win32.cpp
        io_win32.hpp
        io_win32.ipp
        library_win32.hpp
        library_win32.cpp
        time_win32.cpp
        win32/handle.cpp
        win32/handle.hpp
        win32/registry.hpp
        win32/registry.cpp
    )
endif ()


###############################################################################
# Common files
list (
    APPEND UTIL_FILES
    hash/md2.cpp
    adapter.hpp
    adapter.cpp
    algo/sort.cpp
    algo/sort.hpp
    algo/sort.ipp
    alloc/fwd.hpp
    alloc/affix.cpp
    alloc/affix.hpp
    alloc/aligned.hpp
    alloc/allocator.cpp
    alloc/allocator.hpp
    alloc/allocator.ipp
    alloc/arena.cpp
    alloc/arena.hpp
    alloc/arena.ipp
    alloc/dynamic.hpp
    alloc/fallback.cpp
    alloc/fallback.hpp
    alloc/linear.cpp
    alloc/linear.hpp
    alloc/malloc.cpp
    alloc/malloc.hpp
    alloc/null.cpp
    alloc/null.hpp
    alloc/stack.cpp
    alloc/stack.hpp
    annotation.hpp
    ascii.hpp
    backtrace.hpp
    bezier.cpp
    bezier1.cpp
    bezier2.cpp
    bezier3.cpp
    bezier.hpp
    bitwise.cpp
    bitwise.hpp
    cast.hpp
    cmdopt.cpp
    cmdopt.hpp
    cmdopt.ipp
    colour.cpp
    colour.hpp
    colour.ipp
    coord/fwd.hpp
    coord/base.hpp
    coord.hpp
    coord/init.hpp
    coord/iostream.hpp
    coord/names.hpp
    coord/ops.hpp
    coord/store.hpp
    crypto/arc4.cpp
    crypto/arc4.hpp
    crypto/ice.cpp
    crypto/ice.hpp
    crypto/salsa.cpp
    crypto/salsa.hpp
    crypto/tea.cpp
    crypto/tea.hpp
    crypto/xtea.cpp
    crypto/xtea.hpp
    crypto/xxtea.cpp
    crypto/xxtea.hpp
    debug.cpp
    debug.hpp
    debug.ipp
    endian.cpp
    endian.hpp
    except.cpp
    except.hpp
    exe.hpp
    extent.cpp
    extent.hpp
    extent.ipp
    fixed.cpp
    fixed.hpp
    float.cpp
    float.hpp
    format.cpp
    format.hpp
    format.ipp
    fourcc.cpp
    fourcc.hpp
    geom/fwd.hpp
    geom/aabb.cpp
    geom/aabb.hpp
    geom/aabb.ipp
    geom/cylinder.cpp
    geom/cylinder.hpp
    geom/ellipse.cpp
    geom/ellipse.hpp
    geom/ellipse.ipp
    geom/iostream.cpp
    geom/iostream.hpp
    geom/ops.hpp
    geom/plane.cpp
    geom/plane.hpp
    geom/ray.cpp
    geom/ray.hpp
    geom/rect.cpp
    geom/rect.hpp
    geom/sample.hpp
    geom/sample.ipp
    geom/sphere.cpp
    geom/sphere.hpp
    geom/tri.cpp
    geom/tri.hpp
    hash.hpp
    hash/fwd.hpp
    hash/simple.hpp
    hash/simple.cpp
    hash/adler.cpp
    hash/adler.hpp
    hash/bsdsum.cpp
    hash/bsdsum.hpp
    hash/crc.cpp
    hash/crc.hpp
    hash/fasthash.cpp
    hash/fasthash.hpp
    hash/fletcher.cpp
    hash/fletcher.hpp
    hash/fnv1a.cpp
    hash/fnv1a.hpp
    hash/hmac.cpp
    hash/hmac.hpp
    hash/hotp.cpp
    hash/hotp.hpp
    hash/keccak.cpp
    hash/keccak.hpp
    hash/md2.cpp
    hash/md2.hpp
    hash/md4.cpp
    hash/md4.hpp
    hash/md5.cpp
    hash/md5.hpp
    hash/murmur/common.cpp
    hash/murmur/common.hpp
    hash/murmur.hpp
    hash/murmur/murmur1.cpp
    hash/murmur/murmur1.hpp
    hash/murmur/murmur2.cpp
    hash/murmur/murmur2.hpp
    hash/murmur/murmur3.cpp
    hash/murmur/murmur3.hpp
    hash/pbkdf1.cpp
    hash/pbkdf1.hpp
    hash/pbkdf2.cpp
    hash/pbkdf2.hpp
    hash/ripemd.cpp
    hash/ripemd.hpp
    hash/sha1.cpp
    hash/sha1.hpp
    hash/sha2.cpp
    hash/sha2.hpp
    hash/wang.hpp
    hash/wang.ipp
    hash/xxhash.cpp
    hash/xxhash.hpp
    introspection.cpp
    introspection.hpp
    io.cpp
    io.hpp
    io.ipp
    iterator.hpp
    job/queue.cpp
    job/queue.hpp
    json/fwd.hpp
    json/except.cpp
    json/except.hpp
    ${CMAKE_CURRENT_BINARY_DIR}/json/flat.cpp
    json/flat.hpp
    json/schema.cpp
    json/schema.hpp
    json/tree.cpp
    json/tree.hpp
    library.hpp
    log.cpp
    log.hpp
    log.ipp
    maths.cpp
    maths.hpp
    matrix.cpp
    matrix2.cpp
    matrix3.cpp
    matrix4.cpp
    matrix.hpp
    matrix.ipp
    memory/deleter.cpp
    memory/deleter.hpp
    nocopy.hpp
    parse.cpp
    parse.hpp
    pascal.cpp
    pascal.hpp
    platform.hpp
    point.cpp
    point.hpp
    point.ipp
    pointer.hpp
    polynomial.cpp
    polynomial.hpp
    polynomial.ipp
    pool.cpp
    pool.hpp
    pool.ipp
    preprocessor.hpp
    quaternion.cpp
    quaternion.hpp
    quaternion.ipp
    raii.hpp
    rand/lcg.cpp
    rand/lcg.hpp
    rand/xorshift.cpp
    rand/xorshift.hpp
    rand/mwc64x.cpp
    rand/mwc64x.hpp
    random.cpp
    random.hpp
    range.cpp
    range.hpp
    range.ipp
    rational.cpp
    rational.hpp
    rational.ipp
    region.cpp
    region.hpp
    region.ipp
    roots/bisection.hpp
    sarray.cpp
    sarray.hpp
    si.cpp
    signal.cpp
    signal.hpp
    signal.ipp
    si.hpp
    stats.cpp
    stats.hpp
    stream.cpp
    stream.hpp
    string.cpp
    string.hpp
    stringid.cpp
    stringid.hpp
    strongdef.cpp
    strongdef.hpp
    tap.cpp
    tap.hpp
    tap.ipp
    term.cpp
    term.hpp
    time.cpp
    time.hpp
    time.ipp
    tuple.cpp
    tuple.hpp
    typeidx.cpp
    typeidx.hpp
    types/bits.hpp
    types/comparator.hpp
    types.hpp
    types/string.cpp
    types/string.hpp
    types/traits.hpp
    uri.cpp
    uri.hpp
    variadic.cpp
    variadic.hpp
    vector.cpp
    vector.hpp
    vector.ipp
    version.cpp
    version.hpp
    view.cpp
    view.ipp
    view.hpp
)

###############################################################################
add_library(cruft-util ${UTIL_FILES})
target_link_libraries (cruft-util ${LIBS})


###############################################################################
foreach (tool hash json-clean json-schema json-validate scratch)
    add_executable (util_${tool} tools/${tool}.cpp)
    set_target_properties (util_${tool} PROPERTIES OUTPUT_NAME ${tool})
    target_link_libraries (util_${tool} cruft-util)
endforeach ()


###############################################################################
option (TESTS "enable unit testing" ON)

if (TESTS)
    include(CTest)
    enable_testing()

    list (
        APPEND TEST_BIN
        ascii
        algo/sort
        alloc/aligned
        alloc/arena
        alloc/dynamic
        alloc/linear
        alloc/stack
        affine
        backtrace
        bezier
        bitwise
        cmdopt
        colour
        comparator
        coord
        crypto/arc4
        crypto/ice
        crypto/salsa
        crypto/tea
        crypto/xtea
        crypto/xxtea
        exe
        extent
        fixed
        float
        format
        geom/aabb
        geom/ray
        hash/checksum
        hash/crc
        hash/fasthash
        hash/hmac
        hash/hotp
        hash/keccak
        hash/md2
        hash/md4
        hash/md5
        hash/murmur
        hash/ripemd
        hash/sha1
        hash/sha2
        hash/xxhash
        hton
        introspection
        iterator
        job/queue
        json_types
        maths
        matrix
        memory/deleter
        parse
        point
        polynomial
        pool
        quaternion
        rand/buckets
        range
        rational
        region
        roots/bisection
        sarray
        signal
        stream
        string
        stringid
        strongdef
        tuple
        typeidx
        uri
        vector
        version
        view
    )

    foreach(t ${TEST_BIN})
        string(REPLACE "/" "_" name "test/${t}")
        add_executable(util_${name} test/${t}.cpp)
        target_link_libraries(util_${name} PRIVATE cruft-util)
        add_test(NAME util_${name} COMMAND util_${name})
    endforeach(t)
endif ()


###############################################################################
get_directory_property (HAS_PARENT PARENT_DIRECTORY)
if (HAS_PARENT)
    set (CRUFT_UTIL_FOUND 1 PARENT_SCOPE)
endif ()


##-----------------------------------------------------------------------------
configure_file(libcruft-util-system.pc.in libcruft-util.pc)
configure_file(Doxyfile.in Doxyfile)
