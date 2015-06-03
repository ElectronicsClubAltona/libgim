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
#include "noise/turbulence.hpp"
#include "extent.hpp"
#include "colour.hpp"

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
#if 0
    //util::noise::fractal::fbm<float, util::noise::basis::worley<float>> b (seed);
    //util::noise::fractal::rmf<float, util::noise::basis::worley<float>> b (seed);
    //util::noise::fractal::fbm<float, util::noise::basis::perlin<float,util::lerp::cubic>> b (seed);
    //util::noise::fractal::rmf<float, util::noise::basis::perlin<float,util::lerp::cubic>> b (seed);
    //util::noise::fractal::hmf<float, util::noise::basis::perlin<float,util::lerp::cubic>> b (seed);
    util::noise::fractal::hetero<float, util::noise::basis::value<float,util::lerp::quintic>> b (seed);

    b.octaves (8);
    b.frequency (10.f / size.w);
    b.lacunarity = 2.f;
    b.H = 1.0f;
    b.seed (seed);
#else
    util::noise::turbulence<
        float,
        //util::noise::fractal::hetero<float, util::noise::basis::worley<float>>,
        util::noise::fractal::hetero<float, util::noise::basis::perlin<float,util::lerp::cubic>>,
        util::noise::fractal::fbm<float, util::noise::basis::perlin<float,util::lerp::quintic>>
    > b (seed, { 0.13f, 0.13f });

    b.data.frequency (10.f / size.w);
    b.perturb[0].octaves (4);
    b.perturb[1].octaves (4);
    b.perturb[0].frequency (10.f / size.w);
    b.perturb[1].frequency (10.f / size.w);
#endif

    // generate the values. offset positions slightly to observe simple axis issues with perlin basis
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

    // create a coloured map with this gradient
    static const struct {
        float scale;
        util::colour3u value;
    } GRADPOINT[] = {
        {  0 / 32.f, {   0,   0, 128 } },
        { 12 / 32.f, {   0,   0, 255 } },
        { 16 / 32.f, {   0, 128, 255 } },
        { 17 / 32.f, { 240, 240,  64 } },
        { 18 / 32.f, {  32, 160,   0 } },
        { 22 / 32.f, { 224, 224,   0 } },
        { 28 / 32.f, { 128, 128, 128 } },
        { 32 / 32.f, { 255, 255, 255 } },
        { 1000000.f, { 255, 255, 255 } },
    };

    std::unique_ptr<uint8_t[]> coloured (new uint8_t[size.area () * 3]);
    for (size_t i = 0; i < size.area (); ++i) {
        auto  v = img.data ()[i] + 0/32.f;
        auto c1 = std::upper_bound (std::begin (GRADPOINT),
                                    std::end (GRADPOINT),
                                    v,
                                    [] (auto a, auto b) { return a < b.scale; });
        auto c0 = c1-1;

        CHECK_GE (v, c0->scale);
        CHECK_LT (v, c1->scale);
        float t = (v - c0->scale) / (c1->scale - c0->scale);
        CHECK_LIMIT (t, 0, 1);
        auto c = (
            (1 - t) * c0->value.template cast<float> () +
            (    t) * c1->value.template cast<float> ()
        ).template cast<uint8_t> ();

        coloured[i*3+0] = c[0];
        coloured[i*3+1] = c[1];
        coloured[i*3+2] = c[2];
    }

    util::ppm::write (coloured.get (), size.w, size.h, size.w*3, "noise.ppm");

    // write the image to disk
    auto grey = img.clone<uint8_t> ();
    util::pgm::write (grey, "noise.pgm");
}
