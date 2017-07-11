#include <cstdint>
#include <array>

#include "../bitwise.hpp"
#include "../endian.hpp"

namespace util::crypto::salsa {

    ///////////////////////////////////////////////////////////////////////////
    constexpr
    uint32_t
    R (uint32_t a, uint32_t b, uint32_t c, uint32_t k)
    {
        return b ^ util::rotatel (a + c, k);
    }


    ///////////////////////////////////////////////////////////////////////////
    constexpr
    std::array<uint32_t,4>
    quarter (std::array<uint32_t,4> y) noexcept
    {
        std::array<uint32_t,4> z {};

        z[1] = R (y[0], y[1], y[3],  7);
        z[2] = R (z[1], y[2], y[0],  9);
        z[3] = R (z[2], y[3], z[1], 13);
        z[0] = R (z[3], y[0], z[2], 18);

        return z;
    }


    ///////////////////////////////////////////////////////////////////////////
    constexpr
    std::array<uint32_t,16>
    row (const std::array<uint32_t,16> y) noexcept
    {
        const auto [z00, z01, z02, z03] = quarter ({y[ 0], y[ 1], y[ 2], y[ 3]});
        const auto [z05, z06, z07, z04] = quarter ({y[ 5], y[ 6], y[ 7], y[ 4]});
        const auto [z10, z11, z08, z09] = quarter ({y[10], y[11], y[ 8], y[ 9]});
        const auto [z15, z12, z13, z14] = quarter ({y[15], y[12], y[13], y[14]});

        return {
            z00, z01, z02, z03,
            z04, z05, z06, z07,
            z08, z09, z10, z11,
            z12, z13, z14, z15
        };
    }


    ///////////////////////////////////////////////////////////////////////////
    constexpr
    std::array<uint32_t,16>
    col (const std::array<uint32_t,16> x) noexcept
    {
        const auto [y00, y04, y08, y12] = quarter ({x[ 0], x[ 4], x[ 8], x[12]});
        const auto [y05, y09, y13, y01] = quarter ({x[ 5], x[ 9], x[13], x[ 1]});
        const auto [y10, y14, y02, y06] = quarter ({x[10], x[14], x[ 2], x[ 6]});
        const auto [y15, y03, y07, y11] = quarter ({x[15], x[ 3], x[ 7], x[11]});

        return {
            y00, y01, y02, y03,
            y04, y05, y06, y07,
            y08, y09, y10, y11,
            y12, y13, y14, y15,
        };
    }


    ///////////////////////////////////////////////////////////////////////////
    constexpr
    std::array<uint32_t,16>
    doubleround (const std::array<uint32_t,16> x) noexcept
    {
        return row (col (x));
    }
}


namespace util::crypto {
    ///////////////////////////////////////////////////////////////////////////
    std::array<uint8_t,64>
    salsa20 (const std::array<uint8_t,64>) noexcept;
}
