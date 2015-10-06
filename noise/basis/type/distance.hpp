
#ifndef __UTIL_NOISE_BASIS_TYPE_HPP
#define __UTIL_NOISE_BASIS_TYPE_HPP

#include "../../../vector.hpp"
#include "../../../maths.hpp"

#include <array>

namespace util { namespace noise { namespace basis { namespace type {
    template <size_t S, size_t R>
    struct distance {
    protected:
        static constexpr size_t OFFSET_SIZE = util::pow(R*2+1,S);

        static const std::array<
            vectori<S>, util::pow(R*2+1,S)
        > OFFSETS;
    };
} } } }

#endif
