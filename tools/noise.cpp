#include "image.hpp"
#include "noise.hpp"
#include "noise/lerp.hpp"
    
int
main (void)
{
    util::image::buffer<float> img (1920, 1080);

    //static constexpr auto lerp_t = lerp::cubic;
    //using basis_t = util::noise::gradient<lerp_t>;
    //using noise_t = util::noise::fbm<basis_t>;

    using noise_t = util::noise::fbm<util::noise::gradient<double,util::lerp::quintic>>;
    //using noise_t = util::noise::musgrave<util::noise::gradient<double,util::lerp::quintic>>;
    //using noise_t = util::noise::fbm<util::noise::cellular<double>>;

    util::noise::fill (img, noise_t {});
    
    auto grey = img.clone<uint8_t> ();
    util::pgm::write (grey, "noise.ppm");
}
