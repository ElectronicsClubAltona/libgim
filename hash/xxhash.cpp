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

#include "xxhash.hpp"

#include "../bitwise.hpp"
#include "../debug.hpp"
#include "../endian.hpp"

#include <cstring>

using util::hash::xxhash;


///////////////////////////////////////////////////////////////////////////////
template <typename T>
static
T
read_le (const void *ptr)
{
    return *static_cast<const T*> (ptr);
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
struct constants {
    static const T prime[5];
    static const T round_rotl;
};


//-----------------------------------------------------------------------------
template <>
const uint32_t
constants<uint32_t>::prime[5] = {
        2654435761u,
        2246822519u,
        3266489917u,
         668265263u,
         374761393u,
};


//-----------------------------------------------------------------------------
template <>
const uint32_t
constants<uint32_t>::round_rotl = 13;


//-----------------------------------------------------------------------------
template <>
const uint64_t constants<uint64_t>::prime[5] = {
    11400714785074694791u,
    14029467366897019727u,
     1609587929392839161u,
     9650029242287828579u,
     2870177450012600261u,
};


//-----------------------------------------------------------------------------
template <>
const uint64_t
constants<uint64_t>::round_rotl = 31;


///////////////////////////////////////////////////////////////////////////////
template <typename T>
static
T
round (T seed, T input)
{
    seed += input * constants<T>::prime[1];
    seed  = util::rotatel (seed, constants<T>::round_rotl);
    seed *= constants<T>::prime[0];

    return seed;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
xxhash<T>::xxhash (uint32_t _seed):
    m_seed  (_seed)
{ ; }


///////////////////////////////////////////////////////////////////////////////
template <typename T>
typename xxhash<T>::digest_t
xxhash<T>::operator() (const util::view<const uint8_t*> data)
{
    struct {
        uint32_t total_len_32;
        uint32_t large_len;

        T v1, v2, v3, v4;
        uint32_t mem32[4];
        uint32_t memsize;
        uint32_t reserved;

        //uint64_t length;
        //T v[4];
        //T mem[4];
        //unsigned memsize;
    } m_state;

    /* RESET */
    memset (&m_state, 0, sizeof (m_state));

    m_state.v1 = m_seed + constants<T>::prime[0] + constants<T>::prime[1];
    m_state.v2 = m_seed + constants<T>::prime[1];
    m_state.v3 = m_seed;
    m_state.v4 = m_seed - constants<T>::prime[0];

    /* UPDATE */
    do {
        auto first = data.begin ();
        auto last  = data.end ();
        if (first == last)
            break;

        CHECK (first);
        CHECK (last);
        CHECK_LE (first, last);

        //auto endian = XXH_littleEndian;
        size_t len = last - first;
        auto input = (const void*)first;

        auto          p = reinterpret_cast<const uint8_t*> (input);
        auto const bEnd = p + len;

        constexpr auto CHUNK = 4 * sizeof (T);

        m_state.total_len_32 += (unsigned)len;
        m_state.large_len |= (len >= CHUNK) | (m_state.total_len_32 >= CHUNK);

        if (m_state.memsize + len < CHUNK)  {   /* fill in tmp buffer */
            memcpy ((uint8_t*)(m_state.mem32) + m_state.memsize, input, len);
            m_state.memsize += (unsigned)len;
            break;
        }

        if (m_state.memsize) {   /* some data left from previous update */
            memcpy ((uint8_t*)(m_state.mem32) + m_state.memsize, input, CHUNK - m_state.memsize);
            {   const uint32_t* p32 = m_state.mem32;
                m_state.v1 = round<T> (m_state.v1, ltoh (*p32)); p32++;
                m_state.v2 = round<T> (m_state.v2, ltoh (*p32)); p32++;
                m_state.v3 = round<T> (m_state.v3, ltoh (*p32)); p32++;
                m_state.v4 = round<T> (m_state.v4, ltoh (*p32)); p32++;
            }
            p += CHUNK - m_state.memsize;
            m_state.memsize = 0;
        }

        if (p <= bEnd - CHUNK * sizeof (T)) {
            const uint8_t* const limit = bEnd - 4 * sizeof (T);
            T v1 = m_state.v1;
            T v2 = m_state.v2;
            T v3 = m_state.v3;
            T v4 = m_state.v4;

            do {
                v1 = round<T> (v1, read_le<T> (p)); p += sizeof (T);
                v2 = round<T> (v2, read_le<T> (p)); p += sizeof (T);
                v3 = round<T> (v3, read_le<T> (p)); p += sizeof (T);
                v4 = round<T> (v4, read_le<T> (p)); p += sizeof (T);
            } while (p <= limit);

            m_state.v1 = v1;
            m_state.v2 = v2;
            m_state.v3 = v3;
            m_state.v4 = v4;
        }

        if (p < bEnd) {
            memcpy (m_state.mem32, p, (size_t)(bEnd-p));
            m_state.memsize = (unsigned)(bEnd-p);
        }
    } while (0);

    /* DIGEST */
    {
        auto p = reinterpret_cast<const uint8_t*> (m_state.mem32);
        auto last = p + m_state.memsize;

        T h;

        if (m_state.large_len) {
            h = rotatel (m_state.v1, T{ 1}) +
                rotatel (m_state.v2, T{ 7}) +
                rotatel (m_state.v3, T{12}) +
                rotatel (m_state.v4, T{18});
        } else {
            h = m_state.v3 /* == seed */ + constants<T>::prime[4];
        }

        h += m_state.total_len_32;

        while (p + sizeof (T) <= last) {
            h += read_le<T> (p)  * constants<T>::prime[2];
            h  = rotatel (h, 17) * constants<T>::prime[3];
            p += 4;
        }

        while (p < last) {
            h += (*p) * constants<T>::prime[4];
            h  = rotatel (h, 11) * constants<T>::prime[0];
            p++;
        }

        h ^= h >> 15; h *= constants<T>::prime[1];
        h ^= h >> 13; h *= constants<T>::prime[2];
        h ^= h >> 16;

        return h;
    }
}


///////////////////////////////////////////////////////////////////////////////
template class util::hash::xxhash<uint32_t>;
template class util::hash::xxhash<uint64_t>;
