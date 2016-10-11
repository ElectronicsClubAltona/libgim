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

// Derived from Mathew Kwan's 1996 C++ public domain implementation of ICE:
// http://www.darkside.com.au/ice/
//
// M. Kwan, The Design of the ICE Encryption Algorithm, proceedings of Fast
// Software Encryption - Fourth International Workshop, Haifa, Israel,
// Springer-Verlag, pp. 69-82, 1997

#include "./ice.hpp"

#include "../endian.hpp"
#include "../debug.hpp"

#include <cstdint>


///////////////////////////////////////////////////////////////////////////////
/*
 * C++ implementation of the ICE encryption algorithm.
 *
 * Written by Matthew Kwan - July 1996
 */


/* The S-boxes */
static uint32_t ice_sbox[4][1024];
static bool ice_sboxes_initialised = false;


/* Modulo values for the S-boxes */
static
constexpr
uint_fast16_t
ice_smod[4][4] = {
    {333, 313, 505, 369},
    {379, 375, 319, 391},
    {361, 445, 451, 397},
    {397, 425, 395, 505}
};


/* XOR values for the S-boxes */
constexpr
uint8_t
ice_sxor[4][4] = {
    {0x83, 0x85, 0x9b, 0xcd},
    {0xcc, 0xa7, 0xad, 0x41},
    {0x4b, 0x2e, 0xd4, 0x33},
    {0xea, 0xcb, 0x2e, 0x04}
};


/* Permutation values for the P-box */
constexpr
uint32_t
ice_pbox[32] = {
    0x00000001, 0x00000080, 0x00000400, 0x00002000,
    0x00080000, 0x00200000, 0x01000000, 0x40000000,
    0x00000008, 0x00000020, 0x00000100, 0x00004000,
    0x00010000, 0x00800000, 0x04000000, 0x20000000,
    0x00000004, 0x00000010, 0x00000200, 0x00008000,
    0x00020000, 0x00400000, 0x08000000, 0x10000000,
    0x00000002, 0x00000040, 0x00000800, 0x00001000,
    0x00040000, 0x00100000, 0x02000000, 0x80000000
};


/* The key rotation schedule */
constexpr
std::array<uint_fast8_t,8>
ice_keyrot[2] = {
    { 0, 1, 2, 3, 2, 1, 3, 0, },
    { 1, 3, 2, 0, 3, 1, 0, 2, },
};


/*
 * 8-bit Galois Field multiplication of a by b, modulo m.
 * Just like arithmetic multiplication, except that additions and
 * subtractions are replaced by XOR.
 */

template <typename T>
static
T
gf_mult (T a, T b, const T m)
{
    T res = 0;

    while (b) {
        if (b & 1u)
            res ^= a;

        a <<= 1u;
        b >>= 1u;

        if (a >= 256)
            a ^= m;
    }

    return res;
}


/*
 * Galois Field exponentiation.
 * Raise the base to the power of 7, modulo m.
 */

template <typename T>
static
T
gf_exp7 (const T b,
         const T m)
{
    if (b == 0)
        return 0;

    T x;

    x = gf_mult (b, b, m);
    x = gf_mult (b, x, m);
    x = gf_mult (x, x, m);

    return gf_mult (b, x, m);
}


/*
 * Carry out the ICE 32-bit P-box permutation.
 */

static
uint32_t
ice_perm32 (uint32_t x)
{
    uint32_t res = 0;
    const uint32_t *pbox = ice_pbox;

    while (x) {
        if (x & 1)
            res |= *pbox;
        pbox++;
        x >>= 1;
    }

    return res;
}


/*
 * Initialise the ICE S-boxes.
 * This only has to be done once.
 */
static
void
ice_sboxes_init (void)
{
    for (unsigned i = 0; i < 1024; i++) {
        const uint_fast16_t col = (i >> 1) & 0xff;
        const uint_fast16_t row = (i & 0x1) | ((i & 0x200) >> 8);

        for (unsigned j = 0; j < 4; ++j) {
            const auto p = gf_exp7<uint_fast16_t> (
                col ^ ice_sxor[j][row],
                      ice_smod[j][row]
            ) << (24 - j * 8);

            ice_sbox[j][i] = ice_perm32 (static_cast<uint32_t>(p));
        }
    }
}


/*
 * Create a new ICE key.
 */

ice::ice (unsigned n,
          const uint64_t *key_first,
          const uint64_t *key_last)
{
    if (!ice_sboxes_initialised) {
        ice_sboxes_init ();
        ice_sboxes_initialised = true;
    }

    if (n < 1) {
        m_size = 1;
        m_rounds = 8;
    } else {
        m_size = n;
        m_rounds = n * 16;
    }

    m_schedule.resize (m_rounds);

    set (key_first, key_last);
}


/*
 * Destroy an ICE key.
 */

ice::~ice ()
{
    for (auto &s: m_schedule)
        std::fill (std::begin (s), std::end (s), 0);

    m_rounds = m_size = 0;
}


/*
 * The single round ICE f function.
 */

static
uint32_t
ice_f (uint32_t p, const ice::subkey_t &sk)
{
    uint_fast64_t tl, tr; /* Expanded 40-bit values */
    uint_fast64_t al, ar; /* Salted expanded 40-bit values */

    /* Left half expansion */
    tl = ((p >> 16) & 0x3ff) | (((p >> 14) | (p << 18)) & 0xffc00);

    /* Right half expansion */
    tr = (p & 0x3ff) | ((p << 2) & 0xffc00);

    /* Perform the salt permutation */
    // al = (tr & sk->val[2]) | (tl & ~sk->val[2]);
    // ar = (tl & sk->val[2]) | (tr & ~sk->val[2]);
    al = sk[2] & (tl ^ tr);
    ar = al ^ tr;
    al ^= tl;

    al ^= sk[0];               /* XOR with the subkey */
    ar ^= sk[1];

    /* S-box lookup and permutation */
    return (
          ice_sbox[0][al >> 10] | ice_sbox[1][al & 0x3ff]
        | ice_sbox[2][ar >> 10] | ice_sbox[3][ar & 0x3ff]
    );
}


/*
 * Encrypt a block of 8 bytes of data with the given ICE key.
 */
uint64_t
ice::encrypt (const uint64_t _ptext) const
{
    union {
        uint64_t pword;
        uint8_t pbytes[8];
    };

    pword = hton (_ptext);

    uint32_t l, r;

    l =   (((uint32_t) pbytes[0]) << 24u)
        | (((uint32_t) pbytes[1]) << 16u)
        | (((uint32_t) pbytes[2]) <<  8u)
        |              pbytes[3];
    r =   (((uint32_t) pbytes[4]) << 24u)
        | (((uint32_t) pbytes[5]) << 16u)
        | (((uint32_t) pbytes[6]) <<  8u)
        |              pbytes[7];

    for (unsigned i = 0; i < m_rounds; i += 2) {
        l ^= ice_f (r, m_schedule[i]);
        r ^= ice_f (l, m_schedule[i + 1]);
    }

    union {
        uint64_t cword;
        uint8_t cbytes[8];
    };

    for (unsigned i = 0; i < 4; i++) {
        cbytes[3 - i] = r & 0xff;
        cbytes[7 - i] = l & 0xff;

        r >>= 8u;
        l >>= 8u;
    }

    return hton (cword);
}


/*
 * Decrypt a block of 8 bytes of data with the given ICE key.
 */

uint64_t
ice::decrypt (const uint64_t _ctext) const
{
    union {
        uint64_t cword;
        uint8_t cbytes[8];
    };

    cword = hton (_ctext);

    uint32_t l, r;

    l =   (((uint32_t) cbytes[0]) << 24u)
        | (((uint32_t) cbytes[1]) << 16u)
        | (((uint32_t) cbytes[2]) <<  8u)
        |              cbytes[3];
    r =   (((uint32_t) cbytes[4]) << 24u)
        | (((uint32_t) cbytes[5]) << 16u)
        | (((uint32_t) cbytes[6]) <<  8u)
        |              cbytes[7];

    for (int i = m_rounds - 1; i > 0; i -= 2) {
        l ^= ice_f (r, m_schedule[i]);
        r ^= ice_f (l, m_schedule[i - 1]);
    }

    union {
        uint64_t pword;
        uint8_t pbytes[8];
    };

    for (unsigned i = 0; i < 4; i++) {
        pbytes[3 - i] = r & 0xff;
        pbytes[7 - i] = l & 0xff;

        r >>= 8;
        l >>= 8;
    }

    return hton (pword);
}


/*
 * Set 8 rounds [n, n+7] of the key schedule of an ICE key.
 */

void
ice::scheduleBuild (std::array<uint16_t,4> &kb,
                       int       n,
                       const std::array<uint_fast8_t,8> &keyrot)
{
    for (unsigned i = 0; i < 8; i++) {
        int kr = keyrot[i];
        subkey_t &isk = m_schedule[n + i];

        std::fill (std::begin (isk), std::end (isk), 0);

        for (unsigned j = 0; j < 15; j++) {
            uint32_t &curr_sk = isk[j % 3];

            for (unsigned k = 0; k < 4; k++) {
                auto &curr_kb = kb[(kr + k) & 3];
                unsigned bit = curr_kb & 1;

                curr_sk = (curr_sk << 1) | bit;
                curr_kb = (curr_kb >> 1) | ((bit ^ 1) << 15);
            }
        }
    }
}


/*
 * Set the key schedule of an ICE key.
 */

void
ice::set (const uint64_t *_key_first, const uint64_t *_key_last)
{
    (void)_key_last;
    CHECK_EQ ((unsigned)(_key_last - _key_first), m_size);

    auto key = reinterpret_cast<const uint8_t*> (_key_first);

    if (m_rounds == 8) {
        std::array<uint16_t,4> kb;

        for (unsigned i = 0; i < 4; i++)
            kb[3 - i] = (key[i * 2] << 8) | key[i * 2 + 1];

        scheduleBuild (kb, 0, ice_keyrot[0]);
        return;
    }

    for (unsigned i = 0; i < m_size; i++) {
        std::array<uint16_t,4> kb;

        for (unsigned j = 0; j < 4; j++)
            kb[3 - j] = (key[i * 8 + j * 2] << 8) | key[i * 8 + j * 2 + 1];

        scheduleBuild (kb,                i * 8, ice_keyrot[0]);
        scheduleBuild (kb, m_rounds - 8 - i * 8, ice_keyrot[1]);
    }
}


/*
 * Return the key size, in bytes.
 */

unsigned
ice::key_size () const
{
    return (m_size * 8);
}


/*
 * Return the block size, in bytes.
 */

unsigned
ice::block_size () const
{
    return (8);
}
