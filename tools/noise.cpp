#include "image.hpp"
#include "noise.hpp"

#include "noise/fractal/fbm.hpp"
#include "noise/fractal/hetero.hpp"
#include "noise/fractal/hmf.hpp"
#include "noise/fractal/rmf.hpp"
#include "noise/fractal/runtime.hpp"
#include "noise/lerp.hpp"
#include "noise/basis/constant.hpp"
#include "noise/basis/value.hpp"
#include "noise/basis/perlin.hpp"
#include "noise/basis/worley.hpp"
#include "noise/turbulence.hpp"
#include "noise/basis/runtime.hpp"
#include "noise/midpoint.hpp"
#include "extent.hpp"
#include "colour.hpp"
#include "netpbm.hpp"
#include "types.hpp"
#include "cmdopt.hpp"

#include "region.hpp"


///////////////////////////////////////////////////////////////////////////////
template struct util::noise::fractal::fbm<float, util::noise::basis::perlin<float,util::lerp::cubic>>;
template struct util::noise::fractal::hmf<float, util::noise::basis::value<float,util::lerp::cubic>>;
template struct util::noise::fractal::rmf<float, util::noise::basis::constant<float>>;
template struct util::noise::fractal::hetero<float, util::noise::basis::worley<float,2>>;


///////////////////////////////////////////////////////////////////////////////
enum basis_t {
    VALUE,
    PERLIN,
    WORLEY
};


//-----------------------------------------------------------------------------
enum fractal_t {
    FBM,
    HMF,
    RMF,
    HETERO,
};


//-----------------------------------------------------------------------------
enum lerp_t {
    LINEAR,
    CUBIC,
    QUINTIC,
    COSINE,
    TRUNC
};


//-----------------------------------------------------------------------------
std::istream&
operator>> (std::istream &is, basis_t &b)
{
    std::string name;
    is >> name;

    b = name == "value"     ? VALUE  :
        name == "perlin"    ? PERLIN :
        name == "worley"    ? WORLEY :
        (is.setstate (std::istream::failbit), b);

    return is;
}


//-----------------------------------------------------------------------------
std::ostream&
operator<< (std::ostream &os, basis_t b)
{
    switch (b) {
        case VALUE: os << "value"; return os;
        case PERLIN: os << "perlin"; return os;
        case WORLEY: os << "worley"; return os;

        default:
            unreachable ();
    }
}


//-----------------------------------------------------------------------------
std::istream&
operator>> (std::istream &is, fractal_t &f)
{
    std::string name;
    is >> name;

    f = name == "fbm"       ? FBM :
        name == "hmf"       ? HMF :
        name == "rmf"       ? RMF :
        name == "hetero"    ? HETERO :
        (is.setstate (std::istream::failbit), f);

    return is;
}


//-----------------------------------------------------------------------------
std::ostream&
operator<< (std::ostream &os, fractal_t f)
{
    switch (f) {
        case FBM: os << "fbm"; return os;
        case HMF: os << "hmf"; return os;
        case RMF: os << "rmf"; return os;
        case HETERO: os << "hetero"; return os;

        default:
            unreachable ();
    };
}


//-----------------------------------------------------------------------------
std::istream&
operator>> (std::istream &is, lerp_t &l)
{
    std::string name;
    is >> name;

    l = name == "linear"    ? LINEAR    :
        name == "cubic"     ? CUBIC     :
        name == "quintic"   ? QUINTIC   :
        name == "cosine"    ? COSINE    :
        name == "trunc"     ? TRUNC     :
        (is.setstate (std::istream::failbit), l);

    return is;
}


//-----------------------------------------------------------------------------
std::ostream&
operator<< (std::ostream &os, lerp_t &l)
{
    switch (l) {
        case LINEAR:    os << "linear";     return os;
        case CUBIC:     os << "cubic";      return os;
        case QUINTIC:   os << "quintic";    return os;
        case COSINE:    os << "cosine";     return os;
        case TRUNC:     os << "trunc";      return os;

        default:
            unreachable ();
    }
}


///////////////////////////////////////////////////////////////////////////////
int
main (int argc, char **argv)
{
#if 0
    {
        srand (time (nullptr));

        util::extent2u size { 1025 };
        util::image::buffer<float> img (size);
        util::noise::midpoint (img, time (nullptr));

        auto range = std::minmax_element (img.begin (), img.end ());
        auto offset = *range.first;
        auto div    = *range.second - *range.first;
        std::cerr << "range: [" << *range.first << ", " << *range.second << "]\n";

        std::transform (img.begin (), img.end (), img.begin (), [offset,div] (auto i) { return (i - offset) / div; });
        util::pgm::write (img.cast<uint8_t> (), std::cout);
    }

    return 0;
#endif


#ifndef ENABLE_DEBUGGING
    if (isatty (fileno (stdout))) {
        std::cerr << "cowardly refusing to dump binary data to console\n";
        return EXIT_FAILURE;
    }
#endif

    // setup default variables
#ifdef ENABLE_DEBUGGING
    util::extent2u res {320, 240};
#else
    util::extent2u res {1920, 1080};
#endif

    uint64_t seed = time (nullptr);

    basis_t basis = PERLIN;
    fractal_t fractal = FBM;
    lerp_t lerp = QUINTIC;
    unsigned octaves = 8;
    float H;
    float scale = 1.f;
    float turbulence = 0.f;
    unsigned single = 0;

    // fill variables from arguments
    util::cmdopt::parser args;
    args.add<util::cmdopt::option::value<size_t>>    ('w', "width",     "output image width",       res.w);
    args.add<util::cmdopt::option::value<size_t>>    ('h', "height",    "output image height",      res.h);
    args.add<util::cmdopt::option::value<uint64_t>>  ('s', "seed",      "random seed",              seed);
    args.add<util::cmdopt::option::value<basis_t>>   ('b', "basis",     "primary basis function",   basis);
    args.add<util::cmdopt::option::value<fractal_t>> ('f', "fractal",   "primary fractal function", fractal);
    args.add<util::cmdopt::option::value<lerp_t>>    ('l', "lerp",      "interpolation algorithm",  lerp);
    args.add<util::cmdopt::option::value<unsigned>>  ('o', "octaves",   "total fractal iterations", octaves);
    args.add<util::cmdopt::option::count<unsigned>>  ('1', "single",    "single octave",            single);
    args.add<util::cmdopt::option::value<float>>     ('H', "hurst",     "Hurst exponent",           H);
    args.add<util::cmdopt::option::value<float>>     ('x', "scale",     "frequency multiplier",     scale);
    args.add<util::cmdopt::option::value<float>>     ('t', "turbulence","turbulence scale",         turbulence);

    args.scan (argc, argv);

    util::noise::turbulence<
        float,
        util::noise::fractal::runtime<
            float,
            util::noise::basis::runtime<float>
        >,
        util::noise::fractal::fbm<
            float,
            util::noise::basis::perlin<
                float,
                util::lerp::cubic
            >
        >
    > t (seed, { turbulence, turbulence });

    auto &f = t.data;

    switch (fractal) {
        using namespace util::noise;

        case FBM:    f.reset<fractal::fbm   <float,basis::runtime<float>>> (seed); break;
        case HMF:    f.reset<fractal::hmf   <float,basis::runtime<float>>> (seed); break;
        case RMF:    f.reset<fractal::rmf   <float,basis::runtime<float>>> (seed); break;
        case HETERO: f.reset<fractal::hetero<float,basis::runtime<float>>> (seed); break;

        default:
            unreachable ();
    }

    auto &b = f.basis ();
    switch (basis) {
        using namespace util::noise;

        case PERLIN: {
            switch (lerp) {
                case LINEAR:    b.reset<basis::perlin<float,util::lerp::linear>> (seed); break;
                case CUBIC:     b.reset<basis::perlin<float,util::lerp::cubic>> (seed);    break;
                case QUINTIC:   b.reset<basis::perlin<float,util::lerp::quintic>> (seed);  break;
                case COSINE:    b.reset<basis::perlin<float,util::lerp::cosine>> (seed);   break;
                case TRUNC:     b.reset<basis::perlin<float,util::lerp::trunc>> (seed);    break;

            default:
                unreachable ();
            }
            break;
        }

        case VALUE: {
            switch (lerp) {
                case LINEAR:    b.reset<basis::value<float,util::lerp::linear>> (seed); break;
                case CUBIC:     b.reset<basis::value<float,util::lerp::cubic>> (seed);    break;
                case QUINTIC:   b.reset<basis::value<float,util::lerp::quintic>> (seed);  break;
                case COSINE:    b.reset<basis::value<float,util::lerp::cosine>> (seed);   break;
                case TRUNC:     b.reset<basis::value<float,util::lerp::trunc>> (seed);    break;

            default:
                unreachable ();
            }
            break;
        }

        case WORLEY: {
            b.reset<util::noise::basis::worley<float>> (seed);
            break;
        }

        default:
            unreachable ();
    }

    t.seed (seed);
    f.octaves (octaves);
    f.frequency (scale / res.w);
    t.perturb[0].frequency ( scale / res.w);
    t.perturb[1].frequency ( scale / res.w);

    util::image::buffer<float> img (res);

    // XXX: offset slightly to avoid origin artefacts in some basis functions
    static const auto OFFSET = util::vector2f { -100 };

    {
        for (size_t y = 0; y < res.h; ++y)
            for (size_t x = 0; x < res.w; ++x)
                img.data ()[y * img.s + x] = t (util::point2f {float (x), float (y)} + OFFSET);
    }

    // working on the assumption that all octave images are based on summation,
    // subtract the image with one less octave from our current image to leave
    // us with the highest octave contribution only. this is hideously
    // inefficient, but it's not an operation we care about in general.
    if (single && f.octaves () != 1) {
        auto oldoctaves = f.octaves ();
        f.octaves (oldoctaves - 1);
        auto prev = img.clone ();

        for (size_t y = 0; y < res.h; ++y)
            for (size_t x = 0; x < res.w; ++x)
                prev.data ()[y * img.s + x] = t (util::point2f {float (x), float (y)} + OFFSET);

        CHECK_EQ (img.stride (), prev.stride ());
        for (size_t i = 0; i < img.size (); ++i)
            img[i] -= prev[i];

        f.octaves (oldoctaves);
    }

    // rescale into the range [0, 1]
    auto range = std::minmax_element (img.begin (), img.end ());
    auto offset = *range.first;
    auto div    = *range.second - *range.first;

    std::transform (img.begin (), img.end (), img.begin (), [offset,div] (auto i) { return (i - offset) / div; });

    // write the images to disk
    util::pgm::write (img.cast<uint8_t> (), std::cout);
}
