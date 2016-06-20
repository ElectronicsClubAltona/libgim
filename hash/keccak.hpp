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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __HASH_KECCAK_HPP
#define __HASH_KECCAK_HPP

#include <array>
#include <cstdint>

class keccak {
public:
    keccak (unsigned bitrate, unsigned capacity, uint8_t suffix);

    void update (const uint8_t *input, size_t len);
    void digest (uint8_t *output, size_t len);

private:
    void permute (void);

    const unsigned m_bitrate;
    const unsigned m_capacity;
    const uint8_t m_suffix;

    size_t m_cursor;

    union {
        std::array<uint8_t, 200> m_bytes;
        uint64_t m_words[5][5];
    };
};


void FIPS202_SHAKE128(const uint8_t *input, size_t inputByteLen, uint8_t *output, size_t outputByteLen);
void FIPS202_SHAKE256(const uint8_t *input, size_t inputByteLen, uint8_t *output, size_t outputByteLen);
void FIPS202_SHA3_224(const uint8_t *input, size_t inputByteLen, uint8_t *output);
void FIPS202_SHA3_256(const uint8_t *input, size_t inputByteLen, uint8_t *output);
void FIPS202_SHA3_384(const uint8_t *input, size_t inputByteLen, uint8_t *output);
void FIPS202_SHA3_512(const uint8_t *input, size_t inputByteLen, uint8_t *output);

#endif
