#include "image.hpp"
#include "noise.hpp"

#include "noise/fractal/fbm.hpp"
#include "noise/fractal/rmf.hpp"
#include "noise/fractal/hmf.hpp"
#include "noise/fractal/hetero.hpp"
#include "noise/lerp.hpp"
#include "noise/basis/constant.hpp"
#include "noise/basis/value.hpp"
#include "noise/basis/perlin.hpp"
#include "noise/basis/worley.hpp"
#include "extent.hpp"


template struct util::noise::fractal::fbm<float, util::noise::basis::perlin<float,util::lerp::cubic>>;
template struct util::noise::fractal::hmf<float, util::noise::basis::value<float,util::lerp::cubic>>;
template struct util::noise::fractal::rmf<float, util::noise::basis::constant<float>>;
template struct util::noise::fractal::hetero<float, util::noise::basis::worley<float,2>>;

int
main (void)
{
    // setup the output buffer
    util::extent2u size {1920, 1080};
    util::image::buffer<float> img (size);

    uint64_t seed = time (nullptr);

    // setup the noise generator
    //util::noise::fractal::fbm<float, util::noise::basis::worley<float>> b (seed);
    //util::noise::fractal::rmf<float, util::noise::basis::worley<float>> b (seed);
    //util::noise::fractal::fbm<float, util::noise::basis::perlin<float,util::lerp::cubic>> b (seed);
    //util::noise::fractal::rmf<float, util::noise::basis::perlin<float,util::lerp::cubic>> b (seed);
    //util::noise::fractal::hmf<float, util::noise::basis::perlin<float,util::lerp::cubic>> b (seed);
    util::noise::fractal::hetero<float, util::noise::basis::value<float,util::lerp::quintic>> b (seed);

    b.octaves (8);
    b.frequency (10.f / size.w);
    //b.lacunarity = 2.f;
    //b.H = 1.0f;
    b.seed (seed);

    // generate the values. offset positions slightly to avoid simple axis issues with perlin basis
    {
        auto offset = util::vector2f { -100 };

        for (size_t y = 0; y < size.h; ++y)
            for (size_t x = 0; x < size.w; ++x) {
                auto v = b (util::point2f {float (x), float (y)} + offset);
                img.data ()[y * size.w + x] = v;
            }
    }

    // rescale into the range [0, 1]
    auto range = std::minmax_element (img.begin (), img.end ());
    auto offset = *range.first;
    auto div    = *range.second - *range.first;
    std::cerr << "range: [" << *range.first << ", " << *range.second << "]\n";

    std::transform (img.begin (), img.end (), img.begin (), [offset,div] (auto i) { return (i - offset) / div; });

    // write the image to disk
    auto grey = img.clone<uint8_t> ();
    util::pgm::write (grey, "noise.ppm");
}
