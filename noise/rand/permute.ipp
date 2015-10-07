
///////////////////////////////////////////////////////////////////////////////
namespace util { namespace noise { namespace rand {
    template <
        typename U,
        size_t S,
        typename T,
        template <size_t,typename> class Q
    >
    U
    permute::scalar (uint64_t seed, Q<S,T> query) noexcept
    {

        size_t idx = PERMUTE[seed&0xff];
        for (auto i: query)
            idx = PERMUTE[(idx+i)&0xff];
        return PERMUTE[idx] / U(0xff);
    }

    //-------------------------------------------------------------------------
    template <
        template <size_t,typename> class R,
        typename U,
        size_t S,
        typename T,
        template <size_t,typename> class Q
    >
    R<S,U>
    permute::coord (uint64_t seed, Q<S,T> query) noexcept
    {
        auto accum = seed;
        for (auto q: query) {
            size_t idx = q + accum;
            accum = PERMUTE[idx & 0xff];
        }

        R<S,U> res;
        for (size_t i = 0; i < S; ++i) {
            res[i] = PERMUTE[accum];
            res[i] /= 0xff;

            accum = PERMUTE[(accum + i) & 0xff];
        }

        return res;
    }
} } }
