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

#ifndef CRUFT_UTIL_BASE64_HPP
#define CRUFT_UTIL_BASE64_HPP

#include "../view.hpp"

#include "../debug.hpp"

#include <cstdint>
#include <array>


namespace util::encode {
    ///////////////////////////////////////////////////////////////////////////
    template <int Size>
    struct alphabet {
        static const char enc[Size];
        static const uint8_t dec[256];
    };


    ///////////////////////////////////////////////////////////////////////////
    template <>
    const char alphabet<64>::enc[64] = {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
        'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/',
    };


    //-------------------------------------------------------------------------
    template <>
    const uint8_t alphabet<64>::dec[256] = {
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0, 0x3E,    0,    0,    0, 0x3F,
        0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D,    0,    0,    0,    0,    0,    0,
           0, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
        0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,    0,    0,    0,    0,    0,
           0, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
        0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30, 0x31, 0x32, 0x33,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    };


    ///////////////////////////////////////////////////////////////////////////
    template <>
    const char alphabet<32>::enc[32] {
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
        'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
        '2', '3', '4', '5', '6', '7',
    };


    //-------------------------------------------------------------------------
    template <>
    const uint8_t alphabet<32>::dec[256] = {
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0, 0x1a, 0x1b, 0x1c, 0x1d, 0x1e, 0x1f,    0,    0,    0,    0,    0,    0,    0,    0,
           0, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e,
        0x0f, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0
    };


    ///////////////////////////////////////////////////////////////////////////
    template <>
    const char alphabet<16>::enc[16] {
        '0', '1', '2', '3', '4', '5', '6', '7',
        '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
    };

    template <>
    const uint8_t alphabet<16>::dec[256] = {
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
        0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,    0,    0,    0,    0,    0,    0,
           0, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
           0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,    0
    };


    //-------------------------------------------------------------------------
    template <int Size> constexpr auto enc_v = alphabet<Size>::enc;
    template <int Size> constexpr auto dec_v = alphabet<Size>::dec;


    ///////////////////////////////////////////////////////////////////////////
    /// rfc4648 base16, base32, and base64 encoding
    template <int Size>
    struct base {
        static constexpr auto symbol_bits = log2 (Size);
        static constexpr auto group_bits = std::lcm (symbol_bits, 8);
        static constexpr auto group_symbols = group_bits / symbol_bits;
        static constexpr auto group_bytes = group_bits / 8;

        static constexpr uint_fast32_t mask = (1<<symbol_bits)-1;


        template <typename InputT, typename OutputT>
        static OutputT
        encode (OutputT dst, const util::view<InputT> src)
        {
            // convert whole groups of symbols while we have enough bytes remaining
            auto cursor = std::cbegin (src);
            for (size_t i = 0, last = std::size (src) / group_bytes; i != last; ++i) {
                auto tally = std::accumulate (
                    cursor, cursor + group_bytes,
                    uint_fast32_t {0},
                    [] (auto a, auto b) { return a << 8 | b; }
                );

                cursor += group_bytes;

                for (int j = group_symbols - 1; j >= 0; --j)
                    *dst++ = enc_v<Size>[tally >> (j * symbol_bits) & mask];
            }

            // we don't need to pad the output so we can early exit
            if (cursor == std::cend (src))
                return dst;

            // accumulate the remaining bits and pad to a multiple of the
            // output group length
            auto tally = std::accumulate (
                cursor,
                std::cend (src),
                uint_fast32_t {0},
                [] (auto a, auto b) { return a << 8 | b; }
            );

            auto input_remain = std::cend (src) - cursor;
            tally <<= symbol_bits - input_remain * 8 % symbol_bits;

            // write each of the remaining symbols
            auto output_remain = (input_remain * 8 + symbol_bits - 1) / symbol_bits;
            for (auto i = output_remain - 1; i >= 0; --i)
                *dst++ = enc_v<Size>[tally >> (i * symbol_bits) & mask];

            // pad the output to the symbol group size with '=' characters
            auto fill = (group_bytes - input_remain) * 8 / symbol_bits;
            return std::fill_n (dst, fill, '=');
        }


        //---------------------------------------------------------------------
        template <typename InputT, typename OutputT>
        static OutputT
        decode (OutputT dst, util::view<InputT> src)
        {
            if (src.empty ())
                return dst;

            if (src.size () % group_symbols)
                throw std::invalid_argument ("base-encoded strings must be a proper multiple of symbols");

            union {
                uint_fast32_t num;
                uint8_t bytes[group_bytes];
            };

            const bool padded = src.end ()[-1] == '=';
            auto cursor = std::cbegin (src);
            for (size_t i = 0, last = std::size (src) / group_symbols - padded?1:0;
                i != last;
                 ++i)
            {
                num = std::accumulate (
                    cursor, cursor + group_symbols,
                    uint_fast32_t {0},
                    [] (auto a, auto b) { return a << symbol_bits | dec_v<Size>[b]; }
                );

                cursor += group_symbols;

                dst = std::copy (std::crbegin (bytes), std::crend (bytes), dst);
            }

            if (cursor != std::end (src)) {
                auto last = std::find (cursor, std::cend (src), '=');
                num = std::accumulate (
                    cursor, last,
                    uint_fast32_t{0},
                    [] (auto a, auto b) { return a << symbol_bits | dec_v<Size>[b]; }
                );

                auto symbols = last - cursor;
                auto bits = symbols * symbol_bits;
                auto shift = bits%8;
                num >>= shift;

                for (auto i = bits / 8; i; )
                    *dst++ = bytes[--i];
            }

            return dst;
        }
    };
};

#endif
