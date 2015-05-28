#include "image.hpp"
#include "noise.hpp"
#include "noise/lerp.hpp"
#include "noise/basis/value.hpp"
#include "noise/basis/perlin.hpp"
#include "noise/basis/worley.hpp"
#include "extent.hpp"
    
int
main (void)
{
    // setup the output buffer
    util::extent2u size (1920, 1080);
    util::image::buffer<float> img (size);

    // setup the noise generator
    //util::noise::fbm<float, util::noise::basis::worley<float>> b;
    //util::noise::rmf<float, util::noise::basis::worley<float>> b;
    //util::noise::hmf<float, util::noise::basis::perlin<float,util::lerp::quintic>> b;
    util::noise::hetero<float, util::noise::basis::perlin<float,util::lerp::quintic>> b;
    //b.octaves = 3;
    b.frequency = 10.f / size.w;
    b.lacunarity = 2;
    b.basis.seed = time (NULL);

    // generate the values. offset positions slightly to avoid simple axis issues with perlin basis
    for (size_t y = 0; y < size.h; ++y)
        for (size_t x = 0; x < size.w; ++x) {
            auto v = b ({float(x + 20), float(y + 20)});
            img.data ()[y * size.w + x] = v;
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
