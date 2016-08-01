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
 * Copyright:
 *      2014, Danny Robson <danny@nerdcruft.net>
 */

#include "./ripemd.hpp"

#include "../debug.hpp"
#include "../bitwise.hpp"

#include <cstring>
#include <stdexcept>


using util::hash::RIPEMD;

///////////////////////////////////////////////////////////////////////////////
RIPEMD::RIPEMD()
{
    reset ();
}


//-----------------------------------------------------------------------------
void
RIPEMD::reset (void)
{
    m_state[0] = 0x67452301u;
    m_state[1] = 0xEFCDAB89u;
    m_state[2] = 0x98BADCFEu;
    m_state[3] = 0x10325476u;
    m_state[4] = 0xC3D2E1F0u;

    m_length = 0;

    m_buffer.size = 0;
}


///////////////////////////////////////////////////////////////////////////////
void
RIPEMD::update (
    const uint8_t *restrict first,
    const uint8_t *restrict last) noexcept
{
    CHECK_LE (first, last);

    update (first, last - first);
}


//-----------------------------------------------------------------------------
void
RIPEMD::update (const void *restrict _data, size_t len) noexcept
{
    CHECK (_data);

    auto data = static_cast<const uint8_t *restrict> (_data);

    size_t cursor = 0;

    while (cursor < len) {
        size_t width = sizeof (m_buffer.d08) - m_buffer.size;
        size_t chunk = min (width, len - cursor);

        memcpy (m_buffer.d08 + m_buffer.size, data + cursor, chunk);
        m_length      += chunk;
        m_buffer.size += chunk;

        if (m_buffer.size == sizeof (m_buffer.d08))
            transform ();

        cursor += chunk;
    }

    if (m_length >> sizeof (m_length) * 8 - 3 != 0)
        panic ("exceeded maximum message length");
}


///////////////////////////////////////////////////////////////////////////////
static constexpr
uint32_t
f1 (uint32_t x, uint32_t y, uint32_t z)
{
    return x  ^ y ^ z;
}


//-----------------------------------------------------------------------------
static constexpr
uint32_t
f2 (uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) | (~x & z);
}


//-----------------------------------------------------------------------------
static constexpr
uint32_t
f3 (uint32_t x, uint32_t y, uint32_t z)
{
    return (x | ~y) ^ z;
}


//-----------------------------------------------------------------------------
static constexpr
uint32_t
f4 (uint32_t x, uint32_t y, uint32_t z)
{
    return (x & z) | (y & ~z);
}


//-----------------------------------------------------------------------------
static constexpr
uint32_t
f5 (uint32_t x, uint32_t y, uint32_t z)
{
    return x ^ (y | ~z);
}


///////////////////////////////////////////////////////////////////////////////
void
RIPEMD::transform (void)
{
    CHECK_EQ (m_buffer.size, sizeof (m_buffer.d32));

    // Use: boolean function f
    //      state parameters a, b, c, d, e
    //      offset value     o
    //      input data value x
    //      shift length     s

    #define ROUND(f,a,b,c,d,e,o,x,s) {  \
        a += f(b, c, d);                \
        a += m_buffer.d32[x] + o;       \
        a  = rotatel (a, s);            \
        a += e;                         \
        c  = rotatel (c, 10);           \
    }

    #define R1a(a,b,c,d,e,x,s) ROUND(f1,a,b,c,d,e,0x00000000u,x,s)
    #define R2a(a,b,c,d,e,x,s) ROUND(f2,a,b,c,d,e,0x5A827999u,x,s)
    #define R3a(a,b,c,d,e,x,s) ROUND(f3,a,b,c,d,e,0x6ED9EBA1u,x,s)
    #define R4a(a,b,c,d,e,x,s) ROUND(f4,a,b,c,d,e,0x8F1BBCDCu,x,s)
    #define R5a(a,b,c,d,e,x,s) ROUND(f5,a,b,c,d,e,0xA953FD4Eu,x,s)

    #define R1b(a,b,c,d,e,x,s) ROUND(f5,a,b,c,d,e,0x50A28BE6u,x,s)
    #define R2b(a,b,c,d,e,x,s) ROUND(f4,a,b,c,d,e,0x5C4DD124u,x,s)
    #define R3b(a,b,c,d,e,x,s) ROUND(f3,a,b,c,d,e,0x6D703EF3u,x,s)
    #define R4b(a,b,c,d,e,x,s) ROUND(f2,a,b,c,d,e,0x7A6D76E9u,x,s)
    #define R5b(a,b,c,d,e,x,s) ROUND(f1,a,b,c,d,e,0x00000000u,x,s)

    uint32_t a1, b1, c1, d1, e1;
    uint32_t a2, b2, c2, d2, e2;

    a1 = a2 = m_state[0];
    b1 = b2 = m_state[1];
    c1 = c2 = m_state[2];
    d1 = d2 = m_state[3];
    e1 = e2 = m_state[4];

    // Left round 1
    R1a(a1, b1, c1, d1, e1,  0, 11);
    R1a(e1, a1, b1, c1, d1,  1, 14);
    R1a(d1, e1, a1, b1, c1,  2, 15);
    R1a(c1, d1, e1, a1, b1,  3, 12);
    R1a(b1, c1, d1, e1, a1,  4,  5);
    R1a(a1, b1, c1, d1, e1,  5,  8);
    R1a(e1, a1, b1, c1, d1,  6,  7);
    R1a(d1, e1, a1, b1, c1,  7,  9);
    R1a(c1, d1, e1, a1, b1,  8, 11);
    R1a(b1, c1, d1, e1, a1,  9, 13);
    R1a(a1, b1, c1, d1, e1, 10, 14);
    R1a(e1, a1, b1, c1, d1, 11, 15);
    R1a(d1, e1, a1, b1, c1, 12,  6);
    R1a(c1, d1, e1, a1, b1, 13,  7);
    R1a(b1, c1, d1, e1, a1, 14,  9);
    R1a(a1, b1, c1, d1, e1, 15,  8);

    // Left round 2
    R2a(e1, a1, b1, c1, d1,  7,  7);
    R2a(d1, e1, a1, b1, c1,  4,  6);
    R2a(c1, d1, e1, a1, b1, 13,  8);
    R2a(b1, c1, d1, e1, a1,  1, 13);
    R2a(a1, b1, c1, d1, e1, 10, 11);
    R2a(e1, a1, b1, c1, d1,  6,  9);
    R2a(d1, e1, a1, b1, c1, 15,  7);
    R2a(c1, d1, e1, a1, b1,  3, 15);
    R2a(b1, c1, d1, e1, a1, 12,  7);
    R2a(a1, b1, c1, d1, e1,  0, 12);
    R2a(e1, a1, b1, c1, d1,  9, 15);
    R2a(d1, e1, a1, b1, c1,  5,  9);
    R2a(c1, d1, e1, a1, b1,  2, 11);
    R2a(b1, c1, d1, e1, a1, 14,  7);
    R2a(a1, b1, c1, d1, e1, 11, 13);
    R2a(e1, a1, b1, c1, d1,  8, 12);

    // Left round 3
    R3a(d1, e1, a1, b1, c1,  3, 11);
    R3a(c1, d1, e1, a1, b1, 10, 13);
    R3a(b1, c1, d1, e1, a1, 14,  6);
    R3a(a1, b1, c1, d1, e1,  4,  7);
    R3a(e1, a1, b1, c1, d1,  9, 14);
    R3a(d1, e1, a1, b1, c1, 15,  9);
    R3a(c1, d1, e1, a1, b1,  8, 13);
    R3a(b1, c1, d1, e1, a1,  1, 15);
    R3a(a1, b1, c1, d1, e1,  2, 14);
    R3a(e1, a1, b1, c1, d1,  7,  8);
    R3a(d1, e1, a1, b1, c1,  0, 13);
    R3a(c1, d1, e1, a1, b1,  6,  6);
    R3a(b1, c1, d1, e1, a1, 13,  5);
    R3a(a1, b1, c1, d1, e1, 11, 12);
    R3a(e1, a1, b1, c1, d1,  5,  7);
    R3a(d1, e1, a1, b1, c1, 12,  5);

    // Left round 4
    R4a(c1, d1, e1, a1, b1,  1, 11);
    R4a(b1, c1, d1, e1, a1,  9, 12);
    R4a(a1, b1, c1, d1, e1, 11, 14);
    R4a(e1, a1, b1, c1, d1, 10, 15);
    R4a(d1, e1, a1, b1, c1,  0, 14);
    R4a(c1, d1, e1, a1, b1,  8, 15);
    R4a(b1, c1, d1, e1, a1, 12,  9);
    R4a(a1, b1, c1, d1, e1,  4,  8);
    R4a(e1, a1, b1, c1, d1, 13,  9);
    R4a(d1, e1, a1, b1, c1,  3, 14);
    R4a(c1, d1, e1, a1, b1,  7,  5);
    R4a(b1, c1, d1, e1, a1, 15,  6);
    R4a(a1, b1, c1, d1, e1, 14,  8);
    R4a(e1, a1, b1, c1, d1,  5,  6);
    R4a(d1, e1, a1, b1, c1,  6,  5);
    R4a(c1, d1, e1, a1, b1,  2, 12);

    // Left round 5
    R5a(b1, c1, d1, e1, a1,  4,  9);
    R5a(a1, b1, c1, d1, e1,  0, 15);
    R5a(e1, a1, b1, c1, d1,  5,  5);
    R5a(d1, e1, a1, b1, c1,  9, 11);
    R5a(c1, d1, e1, a1, b1,  7,  6);
    R5a(b1, c1, d1, e1, a1, 12,  8);
    R5a(a1, b1, c1, d1, e1,  2, 13);
    R5a(e1, a1, b1, c1, d1, 10, 12);
    R5a(d1, e1, a1, b1, c1, 14,  5);
    R5a(c1, d1, e1, a1, b1,  1, 12);
    R5a(b1, c1, d1, e1, a1,  3, 13);
    R5a(a1, b1, c1, d1, e1,  8, 14);
    R5a(e1, a1, b1, c1, d1, 11, 11);
    R5a(d1, e1, a1, b1, c1,  6,  8);
    R5a(c1, d1, e1, a1, b1, 15,  5);
    R5a(b1, c1, d1, e1, a1, 13,  6);

    // Right round 1
    R1b(a2, b2, c2, d2, e2,  5,  8);
    R1b(e2, a2, b2, c2, d2, 14,  9);
    R1b(d2, e2, a2, b2, c2,  7,  9);
    R1b(c2, d2, e2, a2, b2,  0, 11);
    R1b(b2, c2, d2, e2, a2,  9, 13);
    R1b(a2, b2, c2, d2, e2,  2, 15);
    R1b(e2, a2, b2, c2, d2, 11, 15);
    R1b(d2, e2, a2, b2, c2,  4,  5);
    R1b(c2, d2, e2, a2, b2, 13,  7);
    R1b(b2, c2, d2, e2, a2,  6,  7);
    R1b(a2, b2, c2, d2, e2, 15,  8);
    R1b(e2, a2, b2, c2, d2,  8, 11);
    R1b(d2, e2, a2, b2, c2,  1, 14);
    R1b(c2, d2, e2, a2, b2, 10, 14);
    R1b(b2, c2, d2, e2, a2,  3, 12);
    R1b(a2, b2, c2, d2, e2, 12,  6);

    // Right round 2
    R2b(e2, a2, b2, c2, d2,  6,  9);
    R2b(d2, e2, a2, b2, c2, 11, 13);
    R2b(c2, d2, e2, a2, b2,  3, 15);
    R2b(b2, c2, d2, e2, a2,  7,  7);
    R2b(a2, b2, c2, d2, e2,  0, 12);
    R2b(e2, a2, b2, c2, d2, 13,  8);
    R2b(d2, e2, a2, b2, c2,  5,  9);
    R2b(c2, d2, e2, a2, b2, 10, 11);
    R2b(b2, c2, d2, e2, a2, 14,  7);
    R2b(a2, b2, c2, d2, e2, 15,  7);
    R2b(e2, a2, b2, c2, d2,  8, 12);
    R2b(d2, e2, a2, b2, c2, 12,  7);
    R2b(c2, d2, e2, a2, b2,  4,  6);
    R2b(b2, c2, d2, e2, a2,  9, 15);
    R2b(a2, b2, c2, d2, e2,  1, 13);
    R2b(e2, a2, b2, c2, d2,  2, 11);

    // Right round 3
    R3b(d2, e2, a2, b2, c2, 15,  9);
    R3b(c2, d2, e2, a2, b2,  5,  7);
    R3b(b2, c2, d2, e2, a2,  1, 15);
    R3b(a2, b2, c2, d2, e2,  3, 11);
    R3b(e2, a2, b2, c2, d2,  7,  8);
    R3b(d2, e2, a2, b2, c2, 14,  6);
    R3b(c2, d2, e2, a2, b2,  6,  6);
    R3b(b2, c2, d2, e2, a2,  9, 14);
    R3b(a2, b2, c2, d2, e2, 11, 12);
    R3b(e2, a2, b2, c2, d2,  8, 13);
    R3b(d2, e2, a2, b2, c2, 12,  5);
    R3b(c2, d2, e2, a2, b2,  2, 14);
    R3b(b2, c2, d2, e2, a2, 10, 13);
    R3b(a2, b2, c2, d2, e2,  0, 13);
    R3b(e2, a2, b2, c2, d2,  4,  7);
    R3b(d2, e2, a2, b2, c2, 13,  5);

    // Right round 4
    R4b(c2, d2, e2, a2, b2,  8, 15);
    R4b(b2, c2, d2, e2, a2,  6,  5);
    R4b(a2, b2, c2, d2, e2,  4,  8);
    R4b(e2, a2, b2, c2, d2,  1, 11);
    R4b(d2, e2, a2, b2, c2,  3, 14);
    R4b(c2, d2, e2, a2, b2, 11, 14);
    R4b(b2, c2, d2, e2, a2, 15,  6);
    R4b(a2, b2, c2, d2, e2,  0, 14);
    R4b(e2, a2, b2, c2, d2,  5,  6);
    R4b(d2, e2, a2, b2, c2, 12,  9);
    R4b(c2, d2, e2, a2, b2,  2, 12);
    R4b(b2, c2, d2, e2, a2, 13,  9);
    R4b(a2, b2, c2, d2, e2,  9, 12);
    R4b(e2, a2, b2, c2, d2,  7,  5);
    R4b(d2, e2, a2, b2, c2, 10, 15);
    R4b(c2, d2, e2, a2, b2, 14,  8);

    // Right round 5
    R5b(b2, c2, d2, e2, a2, 12 ,  8);
    R5b(a2, b2, c2, d2, e2, 15 ,  5);
    R5b(e2, a2, b2, c2, d2, 10 , 12);
    R5b(d2, e2, a2, b2, c2,  4 ,  9);
    R5b(c2, d2, e2, a2, b2,  1 , 12);
    R5b(b2, c2, d2, e2, a2,  5 ,  5);
    R5b(a2, b2, c2, d2, e2,  8 , 14);
    R5b(e2, a2, b2, c2, d2,  7 ,  6);
    R5b(d2, e2, a2, b2, c2,  6 ,  8);
    R5b(c2, d2, e2, a2, b2,  2 , 13);
    R5b(b2, c2, d2, e2, a2, 13 ,  6);
    R5b(a2, b2, c2, d2, e2, 14 ,  5);
    R5b(e2, a2, b2, c2, d2,  0 , 15);
    R5b(d2, e2, a2, b2, c2,  3 , 13);
    R5b(c2, d2, e2, a2, b2,  9 , 11);
    R5b(b2, c2, d2, e2, a2, 11 , 11);

    // Finalise state
    d2         = m_state[1] + c1 + d2;
    m_state[1] = m_state[2] + d1 + e2;
    m_state[2] = m_state[3] + e1 + a2;
    m_state[3] = m_state[4] + a1 + b2;
    m_state[4] = m_state[0] + b1 + c2;
    m_state[0] = d2;

    m_buffer.size = 0;
}


///////////////////////////////////////////////////////////////////////////////
void
RIPEMD::finish (void)
{
    // Ensure the length wouldn't overflow if converted to bits. We need to
    // grab this before there's a chance it gets overwritten.
    CHECK_EQ (m_length >> sizeof(m_length) * 8 - 3, 0u);
    uint64_t length = m_length * 8;

    // Push a padding byte into the buffer
    uint8_t padding = 0x80;
    update (&padding, 1);

    // Pad out the line if we couldn't fit a length at the end
    size_t remaining = sizeof (m_buffer.d32) - m_buffer.size;
    if (remaining < 8) {
        static const uint8_t ZEROES[8] = { 0 };

        update (ZEROES, remaining);

        CHECK_EQ (m_buffer.size, 0u);
        remaining = sizeof (m_buffer.d08);
    }

    // Write the length to the end of the buffer
    union {
        uint32_t d32[16];
        uint8_t  d08[64];
    };

    memset (d32, 0, sizeof (d32));
    d32[14] = length & 0xFFFFFFFF;
    d32[15] = length >> 32u;

    // Do the final update
    size_t offset = sizeof(d08) - remaining;
    update (d08 + offset, remaining);
}


///////////////////////////////////////////////////////////////////////////////
RIPEMD::digest_t
RIPEMD::digest (void) const
{
    digest_t d;
    memcpy (d.data (), m_state, sizeof (m_state));
    return d;
}
