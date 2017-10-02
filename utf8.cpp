/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright 2017 Danny Robson <danny@nerdcruft.net>
 */

#include "./utf8.hpp"


///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct test {
    constexpr
    test (T _mask, T _bits) noexcept:
        mask (_mask),
        bits (_bits)
    { ; }

    constexpr bool
    valid (T t) const noexcept
    {
        return (t & mask) == bits;
    }

    constexpr T
    value (T t) const noexcept
    {
        return t & ~mask;
    }

    T mask;
    T bits;
};


//-----------------------------------------------------------------------------
static constexpr test<uint32_t>
operator"" _test (const char *str, size_t len)
{
    uint32_t mask = 0;
    uint32_t bits = 0;

    if (str[0] != '0' || str[1] != 'b')
        throw std::invalid_argument ("invalid bit test prefix");

    for (size_t i = 2; i < len; ++i) {
        auto c = str[i];

        mask <<= 1;
        bits <<= 1;

        switch (c) {
        case '0': mask |= 0x1; bits |= 0x0; break;
        case '1': mask |= 0x1; bits |= 0x1; break;
        case 'x': mask |= 0x0; bits |= 0x0; break;
        default:
            throw std::invalid_argument ("invalid bit test character");
        }

    }

    return { mask, bits };
}


///////////////////////////////////////////////////////////////////////////////
std::vector<util::utf8::codepoint_t>
util::utf8::decode (view<const std::byte*> src)
{
    std::vector<codepoint_t> dst;
    dst.reserve (src.size ());

    static constexpr
    test<codepoint_t> TESTS[] = {
        "0b0xxxxxxx"_test,
        "0b110xxxxx"_test,
        "0b1110xxxx"_test,
        "0b11110xxx"_test
    };

    for (auto cursor = src.cbegin (); cursor != src.cend (); ++cursor) {
        codepoint_t c = std::to_integer<codepoint_t> (*cursor);

        int len = TESTS[0].valid (c) ? 0 :
                  TESTS[1].valid (c) ? 1 :
                  TESTS[2].valid (c) ? 2 :
                  TESTS[3].valid (c) ? 3 :
                  throw malformed_error {};

        if (cursor + len >= src.cend ())
            throw malformed_error {};

        // get the simple ANSI case out of the way
        if (!len) {
            dst.push_back (c);
            continue;
        }

        codepoint_t head = codepoint_t { c & ~TESTS[len].mask } << (len * 6);
        codepoint_t accum = head;
        codepoint_t shift = 0;

        // check every following data byte has the appropriate prefix
        for (int i = 1; i <= len; ++i) {
            if ((std::to_integer<codepoint_t> (cursor[i]) & 0b11'000000u) != 0b10'000000u)
                throw malformed_error {};
        }

        switch (len) {
        case 3: accum |= (std::to_integer<codepoint_t> (cursor[3]) & 0b00111111u) << (shift++ * 6u);
        case 2: accum |= (std::to_integer<codepoint_t> (cursor[2]) & 0b00111111u) << (shift++ * 6u);
        case 1: accum |= (std::to_integer<codepoint_t> (cursor[1]) & 0b00111111u) << (shift++ * 6u);
        }

        // describes the bits required to be present for a valid minimally
        // sized codepoint of a given byte length.
        static constexpr
        codepoint_t LEVEL_MASK[] {
            0b00000000'00000000'01111111,
            0b00000000'00000111'10000000,
            0b00000000'11111000'00000000,
            0b00011111'00000000'00000000
        };

        if (!(accum & LEVEL_MASK[len]))
            throw overlong_error {};

        // utf16 surrogates should not be present in utf8
        if (accum >= 0xD800 && accum <= 0xDFFF)
            throw illegal_codepoint {};

        // reject the BOM
        if (accum == 0xfffe || accum == 0xffff)
            throw illegal_codepoint {};

        dst.push_back (accum);
        std::advance (cursor, len);
    }

    return dst;
}