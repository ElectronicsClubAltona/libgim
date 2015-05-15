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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#include "xxtea.hpp"

#include <cstring>

// test vectors: http://www.cix.co.uk/~klockstone/teavect.htm

using util::crypto::XXTEA;


//-----------------------------------------------------------------------------
static const uint32_t MAGIC = 0x9E3779B9;
static const unsigned ITERATIONS = 32; // each iteration performs two feistel rounds


//-----------------------------------------------------------------------------
static constexpr uint32_t mix (uint32_t Z,
                               uint32_t Y,
                               uint32_t S,
                               size_t E,
                               size_t P,
                               const uint32_t *restrict K)
{
    return ((Z >> 5 ^ Y << 2) + (Y >> 3 ^ Z << 4)) ^ ((S ^ Y) + (K[(P & 3) ^ E] ^ Z));
}


//-----------------------------------------------------------------------------
XXTEA::XXTEA (key_t _key):
    m_key (_key)
{ ; }


//-----------------------------------------------------------------------------
void
XXTEA::encrypt (uint32_t *restrict data, size_t count)
{
    if (count < 2)
        throw std::invalid_argument ("minimum blocksize is 64 bits");

    uint32_t sum = 0;
    uint32_t z = data[count - 1];
    uint32_t y, e, p;

    unsigned rounds = 6 + 52 / count;

    do {
        sum += MAGIC;
        e = (sum >> 2) & 3;

        for (p = 0; p < count - 1; p++) {
            y = data[p + 1];
            z = data[p] += mix (z, y, sum, e, p, m_key.data ());
        }

        y = data[0];
        z = data[count - 1] += mix (z, y, sum, e, p, m_key.data ());
    } while (--rounds);
}


//-----------------------------------------------------------------------------
void
XXTEA::decrypt (uint32_t *restrict data, size_t count)
{
    if (count < 2)
        throw std::invalid_argument ("minimum blocksize is 64 bits");

    uint32_t y, z, sum;
    unsigned p, rounds, e;

    rounds = 6 + 52 / count;
    sum = rounds * MAGIC;
    y = data[0];

    do {
        e = (sum >> 2) & 3;

        for (p = count - 1; p > 0; p--) {
            z = data[p - 1];
            y = data[p    ] -= mix (z, y, sum, e, p, m_key.data ());
        }

        z = data[count - 1];
        y = data[        0] -= mix (z, y, sum, e, p, m_key.data ());

        sum -= MAGIC;
    } while (--rounds);
}
