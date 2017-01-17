list (
    APPEND TEST_BIN
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
    coord
    crypto/arc4
    crypto/ice
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
    json_types
    maths
    matrix
    memory/deleter
    point
    polynomial
    pool
    quaternion
    rand/buckets
    range
    rational
    region
    roots/bisection
    signal
    stream
    string
    stringid
    strongdef
    tuple
    uri
    vector
    version
    view
)


function(define_test path)
    string(REPLACE "/" "_" name "${path}")
    add_executable(${name} "${path}.cpp")
    target_link_libraries(${name} cruft-util stdc++fs)
    add_test(NAME ${name} COMMAND ${name})
endfunction(define_test)


foreach(t ${TEST_BIN})
    define_test(${t})
endforeach(t)
