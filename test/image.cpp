#include "image.hpp"
#include "tap.hpp"

#include <cstdint>

int
main (void)
{
    util::TAP::logger tap;

    constexpr size_t W = 64;
    constexpr size_t H = 128;
    util::image::buffer<1,uint16_t> img ({W, H});

    if (!img.is_packed ())
        tap.skip ("linear position probe requires packed image allocation");
    else {
        // write out sequential values at each pixel, row-major
        size_t i = 0;
        for (size_t y = 0; y < H; ++y)
            for (size_t x = 0; x < W; ++x)
                img[{x,y}] = i++;

        // check pixel values are sequential in row-major order
        bool success = true;
        for (size_t j = 0; j < i; ++j)
            success = success && img[j] == j;

        tap.expect (success, "linear position probe");
    }

    return tap.status ();
}
