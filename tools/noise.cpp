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


void
hydraulic_erode (util::image::buffer<float> &height, const unsigned ticks)
{
    CHECK_GT (ticks, 0);

    std::cerr << "hydraulic_erosion\n";

    const float RAINFALL    = 0.001f;  // quantity of water added each tick
    const float SOLUBILITY  = 0.1001f; // quantity of land picked up by unit rain
    const float EVAPORATION = RAINFALL * .85f; // quantity of water evaporated each tick

    auto water = height.alloc ();
    CHECK (water.is_packed ());
    CHECK (height.is_packed ());

    std::fill (water.begin (), water.end (), 0);

    for (unsigned t = 0; t < ticks; ++t) {
        // apply new rain and erosion
        for (auto &w: water)
            w += RAINFALL;
        for (auto &h: height)
            h -= RAINFALL * SOLUBILITY;

        float total = 0.f;

        // move water to lowest neighbour cell
        for (size_t y = 1; y < height.h - 1; ++y)
            for (size_t x = 1; x < height.w - 1; ++x) {
                const size_t indices[9] = {
                    (y - 1) * height.s + (x - 1),
                    (y - 1) * height.s + (x + 0),
                    (y - 1) * height.s + (x + 1),

                    (y + 0) * height.s + (x - 1),
                    (y + 0) * height.s + (x + 0),
                    (y + 0) * height.s + (x + 1),

                    (y + 1) * height.s + (x - 1),
                    (y + 1) * height.s + (x + 0),
                    (y + 1) * height.s + (x + 1),
                };

                const float level[9] = {
                    height[indices[0]] + water[indices[0]],
                    height[indices[1]] + water[indices[1]],
                    height[indices[2]] + water[indices[2]],

                    height[indices[3]] + water[indices[3]],
                    height[indices[4]] + water[indices[4]],
                    height[indices[5]] + water[indices[5]],

                    height[indices[6]] + water[indices[6]],
                    height[indices[7]] + water[indices[7]],
                    height[indices[8]] + water[indices[8]],
                };

                size_t dst = std::min_element (std::begin (level), std::end (level)) - level;
                if (dst == 4) // if centre, bail
                    continue;

                // transfer as much water as possible to even the heights
                float total_diff = level[4] - level[dst];
                float transfer;
                if (total_diff > water[4])
                    transfer = water[4];
                else
                    transfer = (water[4] - total_diff) / 2;

                water[indices[  4]] -= transfer;
                water[indices[dst]] += transfer;

                total += transfer;
            }

        // evaporate water, deposit sediment
        for (size_t i = 0; i < water.extent ().area (); ++i) {
            water[i]  -= EVAPORATION;
            height[i] += SOLUBILITY * EVAPORATION;
        }
    }

    // forcibly evaporate all remaining water.
    CHECK_EQ (water.extent ().area (), height.extent ().area ());
    for (size_t i = 0; i < water.extent ().area (); ++i)
        height[i] += water[i] * SOLUBILITY;
}


/// talus: maximum desired slope in radians. [0, 90]
/// resistance: proportion of world to keep. [1, 0]
/// scale: horizontal distance scaling factor (relative to vertical)
void
thermal_erode (util::image::buffer<float> &height,
               const float talus,
               const float scale,
               const float resistance)
{
    CHECK_LIMIT (talus, 0, to_radians (90.f));
    CHECK_GT (scale, 0);
    CHECK_LIMIT (resistance, 0, 1);

    // maximum height difference
    const auto maxdiff = scale * std::atan (talus);
    float total = 0.f;

    for (size_t y = 1; y < height.h - 1; ++y)
        for (size_t x = 1; x < height.w - 1; ++x) {
            float centre = height[y * height.s + x];

            float h[9] = {
                height[(y - 1) * height.s + (x - 1)],
                height[(y - 1) * height.s + (x + 0)],
                height[(y - 1) * height.s + (x + 1)],

                height[(y + 0) * height.s + (x - 1)],
                height[(y + 0) * height.s + (x + 0)],
                height[(y + 0) * height.s + (x + 1)],

                height[(y + 1) * height.s + (x - 1)],
                height[(y + 1) * height.s + (x + 0)],
                height[(y + 1) * height.s + (x + 1)],
            };

            float diff = 0.f;
            unsigned dests = 0;
            for (size_t i = 0; i < elems (h); ++i) {
                if (h[i] < centre - maxdiff) {
                    dests++;
                    diff = std::max (diff, centre - h[i]);
                }
            }

            if (diff < maxdiff)
                continue;

            float size = diff * (1 - resistance) / 2;
            float dist = size / dests;

            total += size;
            height[y * height.s + x] -= size;

            if (h[0] < centre) height[(y - 1) * height.s + (x - 1)] += dist;
            if (h[1] < centre) height[(y - 1) * height.s + (x + 0)] += dist;
            if (h[2] < centre) height[(y - 1) * height.s + (x + 1)] += dist;

            if (h[3] < centre) height[(y + 0) * height.s + (x - 1)] += dist;
            if (h[5] < centre) height[(y + 0) * height.s + (x + 1)] += dist;

            if (h[6] < centre) height[(y + 1) * height.s + (x - 1)] += dist;
            if (h[7] < centre) height[(y + 1) * height.s + (x + 0)] += dist;
            if (h[8] < centre) height[(y + 1) * height.s + (x + 1)] += dist;
        }
}


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


// offset a map so a fixed percentage of tiles are below the water height
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
        buckets[size_t (h * 255u)]++;

    size_t pivot = 0;
    for (size_t accum = 0, target = size_t (percentage * height.extent ().area ()); pivot < buckets.size (); ++pivot) {
        accum += buckets[pivot];
        if (accum > target)
            break;
    }

    std::cerr << "ocean pivot: " << pivot << '\n';

    float offset = WATER_HEIGHT - pivot / 256.f;
    for (auto &h: height)
        h += offset;
}



static const unsigned THERMAL_ITERATIONS = 10;
static const unsigned HYDRAULIC_ITERATIONS = 100;

#include "cmdopt.hpp"


int
main (int argc, char **argv)
{
    // setup default variables
#ifdef ENABLE_DEBUGGING
    util::extent2u res {320, 240};
#else
    util::extent2u res {1920, 1080};
#endif

    uint64_t seed = time (nullptr);

    // fill variables from arguments
    util::cmdopt::parser args;
    args.add<util::cmdopt::option::value<size_t>> ('w', "width",  "output image width",  res.w);
    args.add<util::cmdopt::option::value<size_t>> ('h', "height", "output image height", res.h);
    args.add<util::cmdopt::option::value<uint64_t>> ('s', "seed", "random seed", seed);
    //args.add<util::cmdopt::option::value<unsigned>> ('o', "octaves", "total fractal iterations", octaves);
    //args.add<util::cmdopt::option::value<float>> ('H', "hurst", "Hurst exponent", H);

    args.scan (argc, argv);

    util::image::buffer<float> img (res);

    // setup the noise generator
#if 0
    //util::noise::fractal::fbm<float, util::noise::basis::worley<float>> b (seed);
    //util::noise::fractal::rmf<float, util::noise::basis::worley<float>> b (seed);
    //util::noise::fractal::fbm<float, util::noise::basis::perlin<float,util::lerp::cubic>> b (seed);
    //util::noise::fractal::rmf<float, util::noise::basis::perlin<float,util::lerp::cubic>> b (seed);
    //util::noise::fractal::hmf<float, util::noise::basis::perlin<float,util::lerp::cubic>> b (seed);
    util::noise::fractal::hetero<float, util::noise::basis::value<float,util::lerp::quintic>> b (seed);

    b.octaves (8);
    b.frequency (10.f / res.w);
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

    b.data.frequency (1.f / res.w);
    b.perturb[0].octaves (4);
    b.perturb[1].octaves (4);
    b.perturb[0].frequency (10.f / res.w);
    b.perturb[1].frequency (10.f / res.w);
#endif

    // generate the values. offset positions slightly to observe simple axis issues with perlin basis
    {
        auto offset = util::vector2f { -100 };

        for (size_t y = 0; y < res.h; ++y)
            for (size_t x = 0; x < res.w; ++x) {
                auto v = b (util::point2f {float (x), float (y)} + offset);
                img.data ()[y * res.w + x] = v;
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
    write_map (img, "raw");

    auto soft = img.clone ();

    std::cerr << "thermal_erosion\n";
    for (size_t i = 0; i < THERMAL_ITERATIONS; ++i)
        thermal_erode (soft, to_radians (30.f), 1.f / res.w, 0.f);

    hydraulic_erode (soft, HYDRAULIC_ITERATIONS);

    write_map (soft, "soft");
}
