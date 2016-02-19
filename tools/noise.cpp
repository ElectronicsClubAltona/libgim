#include "image/buffer.hpp"
#include "noise.hpp"

#include "noise/fractal/fbm.hpp"
#include "noise/fractal/hetero.hpp"
#include "noise/fractal/hmf.hpp"
#include "noise/fractal/rmf.hpp"
#include "noise/fractal/runtime.hpp"
#include "noise/lerp.hpp"
#include "noise/basis/constant.hpp"
#include "noise/basis/gradient/exp.hpp"
#include "noise/basis/value.hpp"
#include "noise/basis/patch.hpp"
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
#include "hash.hpp"
#include "region.hpp"


constexpr size_t S = 2;

///////////////////////////////////////////////////////////////////////////////
template struct util::noise::fractal::rmf<util::noise::basis::constant<S,float>>;

template struct util::noise::fractal::fbm<
    util::noise::basis::perlin<
        S,float,util::lerp::cubic,util::noise::basis::gradient::uniform
    >
>;

template struct util::noise::fractal::fbm<
    util::noise::basis::perlin<
        S,float,util::lerp::quintic,util::noise::basis::gradient::exp
    >
>;

template struct util::noise::fractal::hmf<
    util::noise::basis::value<
        S,float,util::lerp::cubic
    >
>;

template struct util::noise::fractal::hetero<util::noise::basis::worley<S,float>>;


///////////////////////////////////////////////////////////////////////////////
enum basis_t {
    VALUE,
    PERLIN,
    WORLEY,
    PATCH,
    EXPDIST
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

    b = name == "value"     ? VALUE     :
        name == "perlin"    ? PERLIN    :
        name == "worley"    ? WORLEY    :
        name == "patch"     ? PATCH     :
        name == "expgrad"   ? EXPDIST   :
        (is.setstate (std::istream::failbit), b);

    return is;
}


//-----------------------------------------------------------------------------
std::ostream&
operator<< (std::ostream &os, basis_t b)
{
    switch (b) {
        case VALUE:     os << "value";      return os;
        case PERLIN:    os << "perlin";     return os;
        case WORLEY:    os << "worley";     return os;
        case PATCH:     os << "patch";      return os;
        case EXPDIST:   os << "expgrad";    return os;

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
    // setup default variables
#ifdef ENABLE_DEBUGGING
    util::extent2u res {320, 240};
#else
    util::extent2u res {1920, 1080};
#endif

    srand (time (nullptr));
    uint64_t seed = time (nullptr);

    basis_t basis = PERLIN;
    fractal_t fractal = FBM;
    lerp_t lerp = QUINTIC;
    unsigned octaves = 8;
    float H = std::numeric_limits<float>::quiet_NaN ();
    float lacunarity = std::numeric_limits<float>::quiet_NaN ();
    float amplitude = std::numeric_limits<float>::quiet_NaN ();
    float gain = std::numeric_limits<float>::quiet_NaN ();
    float offset = std::numeric_limits<float>::quiet_NaN ();
    float scale = 1.f;
    float turbulence = 0.f;
    unsigned single = 0;
    float width = 0;

    // fill variables from arguments
    util::cmdopt::parser args;
    args.add<util::cmdopt::option::value<size_t>>    ('w', "width",         "output image width",       res.w);
    args.add<util::cmdopt::option::value<size_t>>    ('h', "height",        "output image height",      res.h);
    args.add<util::cmdopt::option::value<uint64_t>>  ('s', "seed",          "random seed",              seed);
    args.add<util::cmdopt::option::value<basis_t>>   ('b', "basis",         "primary basis function",   basis);
    args.add<util::cmdopt::option::value<fractal_t>> ('f', "fractal",       "primary fractal function", fractal);
    args.add<util::cmdopt::option::value<lerp_t>>    ('l', "lerp",          "interpolation algorithm",  lerp);
    args.add<util::cmdopt::option::value<unsigned>>  ('o', "octaves",       "total fractal iterations", octaves);
    args.add<util::cmdopt::option::count<unsigned>>  ('1', "single",        "single octave",            single);
    args.add<util::cmdopt::option::value<float>>     ('H', "hurst",         "Hurst exponent",           H);
    args.add<util::cmdopt::option::value<float>>     ('G', "gain",          "octave gain",              gain);
    args.add<util::cmdopt::option::value<float>>     ('A', "amplitude",     "base amplitude",           amplitude);
    args.add<util::cmdopt::option::value<float>>     ('L', "lacunarity",    "frequency multiplier",     lacunarity);
    args.add<util::cmdopt::option::value<float>>     ('x', "scale",         "frequency multiplier",     scale);
    args.add<util::cmdopt::option::value<float>>     ('O', "offset",        "hetero offset",            offset);
    args.add<util::cmdopt::option::value<float>>     ('t', "turbulence",    "turbulence scale",         turbulence);
    args.add<util::cmdopt::option::value<float>>     ('W', "patch-width",   "patch blur width",         width);

    args.scan (argc, argv);

#if !defined(ENABLE_DEBUGGING) and !defined(PLATFORM_WIN32)
    if (isatty (fileno (stdout))) {
        std::cerr << "cowardly refusing to dump binary data to console\n";
        return EXIT_FAILURE;
    }
#endif

    util::noise::turbulence<
        util::noise::fractal::runtime<
            util::noise::basis::runtime<S,float>
        >,
        util::noise::fractal::fbm<
            util::noise::basis::perlin<
                S,float,
                util::lerp::cubic
            >
        >
    > t (seed, util::vectorf<S> (turbulence));

    auto &f = t.data;

    switch (fractal) {
        using namespace util::noise;

        case FBM:    f.reset<fractal::fbm<basis::runtime<S,float>>> (seed); break;
        case HMF:    f.reset<fractal::hmf<basis::runtime<S,float>>> (seed); break;
        case RMF:    f.reset<fractal::rmf<basis::runtime<S,float>>> (seed); break;
        case HETERO: {
            auto &child = f.reset<fractal::hetero<basis::runtime<S,float>>> (seed);
            if (!std::isnan (offset))
                child.offset (offset);
            break;
        }

        default:
            unreachable ();
    }

    auto &b = f.basis ();
    switch (basis) {
        using namespace util::noise;

        case PERLIN: {
            switch (lerp) {
                case LINEAR:    b.reset<basis::perlin<S,float,util::lerp::linear>> (seed); break;
                case CUBIC:     b.reset<basis::perlin<S,float,util::lerp::cubic>> (seed);    break;
                case QUINTIC:   b.reset<basis::perlin<S,float,util::lerp::quintic>> (seed);  break;
                case COSINE:    b.reset<basis::perlin<S,float,util::lerp::cosine>> (seed);   break;
                case TRUNC:     b.reset<basis::perlin<S,float,util::lerp::truncate>> (seed);    break;

            default:
                unreachable ();
            }
            break;
        }

        case EXPDIST: {
            switch (lerp) {
                case LINEAR:    b.reset<
                                    basis::perlin<
                                        S,float,util::lerp::linear,basis::gradient::exp
                                    >
                                > (seed); break;

                case CUBIC:     b.reset<basis::perlin<S,float,util::lerp::cubic,basis::gradient::exp>> (seed);    break;
                case QUINTIC:   b.reset<basis::perlin<S,float,util::lerp::quintic,basis::gradient::exp>> (seed);  break;
                case COSINE:    b.reset<basis::perlin<S,float,util::lerp::cosine,basis::gradient::exp>> (seed);   break;
                case TRUNC:     b.reset<basis::perlin<S,float,util::lerp::truncate,basis::gradient::exp>> (seed);    break;

            default:
                unreachable ();
            }
            break;
        }

        case VALUE: {
            switch (lerp) {
                case LINEAR:    b.reset<basis::value<S,float,util::lerp::linear>> (seed); break;
                case CUBIC:     b.reset<basis::value<S,float,util::lerp::cubic>> (seed);    break;
                case QUINTIC:   b.reset<basis::value<S,float,util::lerp::quintic>> (seed);  break;
                case COSINE:    b.reset<basis::value<S,float,util::lerp::cosine>> (seed);   break;
                case TRUNC:     b.reset<basis::value<S,float,util::lerp::truncate>> (seed);    break;

            default:
                unreachable ();
            }
            break;
        }

        case WORLEY: {
            b.reset<util::noise::basis::worley<S,float>> (seed);
            break;
        }

        case PATCH: {
            b.reset<util::noise::basis::patch<S,float>> (seed, width);
            break;
        }

        default:
            unreachable ();
    }

    t.seed (seed);
    f.octaves (octaves);
    f.frequency (scale / res.w);
    if (!std::isnan (H)) f.H (H);
    if (!std::isnan (lacunarity)) f.lacunarity (lacunarity);
    if (!std::isnan (amplitude)) f.amplitude (amplitude);
    if (!std::isnan (gain)) f.gain (gain);

    for (auto &p: t.perturb)
        p.frequency (scale / res.w);

    util::image::buffer<1,float> img (res);

    // XXX: offset slightly to avoid origin artefacts in some basis functions
    const auto OFFSET = util::vector2f {
        (util::hash::mix (                 seed)  & 0xFFFF) / float (0xFFFF),
        (util::hash::mix (util::hash::mix (seed)) & 0xFFFF) / float (0xFFFF)
    } / f.frequency ();

    {
        for (size_t y = 0; y < res.h; ++y)
            for (size_t x = 0; x < res.w; ++x) {
                util::point2f p (x, y);
                img[{x, y}] = t ((p + OFFSET).redim<S> ());
            }
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
            for (size_t x = 0; x < res.w; ++x) {
                util::point2f p (x, y);
                prev[{x,y}] = t ((p + OFFSET).redim<S> ());
            }

        CHECK_EQ (img.stride (), prev.stride ());
        for (size_t i = 0; i < img.size (); ++i)
            img[i] -= prev[i];

        f.octaves (oldoctaves);
    }

    // rescale into the range [0, 1]
    auto range = std::minmax_element (img.begin (), img.end ());
    auto inc = *range.first;
    auto div = *range.second - *range.first;

    std::cerr << '[' << *range.first << ',' << *range.second << "]\n";
    std::transform (img.begin (), img.end (), img.begin (), [inc,div] (auto i) { return (i - inc) / div; });

    // write the images to disk
    util::pgm::write (img.cast<uint8_t> (), std::cout);
}
