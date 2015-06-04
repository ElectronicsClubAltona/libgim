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
#include "netpbm.hpp"
#include "types.hpp"

template struct util::noise::fractal::fbm<float, util::noise::basis::perlin<float,util::lerp::cubic>>;
template struct util::noise::fractal::hmf<float, util::noise::basis::value<float,util::lerp::cubic>>;
template struct util::noise::fractal::rmf<float, util::noise::basis::constant<float>>;
template struct util::noise::fractal::hetero<float, util::noise::basis::worley<float,2>>;


// create a coloured map with this gradient (from libnoise tut3)
static const struct {
    float scale;
    util::colour3u value;
} GRADPOINT[] = {
    {-1000000.f, {   0,   0, 128 } },
    {  0 / 32.f, {   0,   0, 128 } }, // deeps
    { 12 / 32.f, {   0,   0, 255 } }, // shallow
    { 16 / 32.f, {   0, 128, 255 } }, // shore
    { 17 / 32.f, { 240, 240,  64 } }, // sand
    { 18 / 32.f, {  32, 160,   0 } }, // grass
    { 22 / 32.f, { 224, 224,   0 } }, // dirt
    { 28 / 32.f, { 128, 128, 128 } }, // rock
    { 32 / 32.f, { 255, 255, 255 } }, // snow
    { 1000000.f, { 255, 255, 255 } },
};


void
write_map (const util::image::buffer<float> &map, boost::filesystem::path name)
{
    std::unique_ptr<uint8_t[]> coloured (new uint8_t[map.w * map.h * 3]);
    for (size_t i = 0; i < map.w * map.h; ++i) {
        auto  v = map[i] + 0/32.f;
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

    name.replace_extension (".ppm");
    util::ppm::write (coloured.get (), map.w, map.h, map.w*3, name);

    // write the image to disk
    auto grey = map.clone<uint8_t> ();
    name.replace_extension (".pgm");
    util::pgm::write (grey, name);
}


void
adjust_ocean (util::image::buffer<float> &height,
              const float percentage = .2f)
{
    static const float WATER_HEIGHT = 0.5f;

    CHECK_LIMIT (percentage, 0, 1);
    // we assume fully packed data for iteration purposes
    CHECK (height.is_packed ());

    std::array<unsigned,256> buckets{ 0 };
    for (const auto h: height)
        buckets[h * 255]++;

    size_t pivot = 0;
    for (size_t accum = 0, target = percentage * height.area (); pivot < buckets.size (); ++pivot) {
        accum += buckets[pivot];
        if (accum > target)
            break;
    }

    std::cerr << "pivot: " << pivot << '\n';

    float offset = WATER_HEIGHT - pivot / 256.f;
    for (auto &h: height)
        h += offset;
}



int
main (void)
{
    // setup the output buffer
#ifdef ENABLE_DEBUGGING
    util::extent2u size {320, 240};
#else
    util::extent2u size {1920, 1080};
#endif
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

    b.data.frequency (1.f / size.w);
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

    // ensure the ocean isn't too big
    adjust_ocean (img, 0.2f);

    // write the images to disk
    write_map (img, "noise");

}
