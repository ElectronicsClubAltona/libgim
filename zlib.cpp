#include "zlib.hpp"

#include "debug.hpp"

const char *
util::zlib::version (void) {
    return zlibVersion ();
}


size_t
util::zlib::uncompress (uint8_t       *dst, size_t dst_len,
                        const uint8_t *src, size_t src_len) {
    size_t actual_len = dst_len;
    int err = ::uncompress (dst, &actual_len, src, src_len);
    error::try_code (err);
    return actual_len;
}


void
util::zlib::error::try_code (int code) {
    if (Z_OK == code)
        return;

    throw_code(code);
}


util::zlib::error::error (const std::string &_what):
    runtime_error (_what)
{ ; }


void
util::zlib::error::throw_code (int code) {
    CHECK_NEQ (code, Z_OK);

    switch (code) {
        #define do_code(c) case c: throw code_error<c> ();

        do_code(Z_STREAM_ERROR);
        do_code(Z_DATA_ERROR);
        do_code(Z_MEM_ERROR);
        do_code(Z_BUF_ERROR);
        do_code(Z_VERSION_ERROR);
    
        default:
            panic ("Unknown code");
    }
}

