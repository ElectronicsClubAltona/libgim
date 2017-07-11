#include "./salsa.hpp"


///////////////////////////////////////////////////////////////////////////////
std::array<uint8_t,64>
util::crypto::salsa20 (const std::array<uint8_t,64> bytes) noexcept
{
    auto x = *reinterpret_cast<const std::array<uint32_t,16>*> (&bytes);
    auto y = x;

    for (auto &t: x)
        t = util::ltoh (t);

    for (int i = 0; i < 10; ++i)
        x = salsa::doubleround (x);

    for (size_t i = 0; i < std::size (y); ++i)
        x[i] += y[i];

    for (auto &t: x)
        t = util::htol (t);

    return *reinterpret_cast<std::array<uint8_t,64>*> (&x);
}
