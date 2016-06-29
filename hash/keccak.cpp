#include "./keccak.hpp"

#include "../endian.hpp"
#include "../maths.hpp"
#include "../bitwise.hpp"

#include <cstdlib>
#include <cstdint>
#include <array>
#include <iostream>

#if 0
#define FOR(i,n) for(i=0; i<n; ++i)

int LFSR86540(u8 *R) { (*R)=((*R)<<1)^(((*R)&0x80)?0x71:0); return ((*R)&2)>>1; }
#define ROL(a,o) ((((u64)a)<<o)^(((u64)a)>>(64-o)))
static u64 load64(const u8 *x) { ui i; u64 u=0; FOR(i,8) { u<<=8; u|=x[7-i]; } return u; }
static void store64(u8 *x, u64 u) { ui i; FOR(i,8) { x[i]=u; u>>=8; } }
static void xor64(u8 *x, u64 u) { ui i; FOR(i,8) { x[i]^=u; u>>=8; } }
#define rL(x,y) load64((u8*)s+8*(x+5*y))
#define wL(x,y,l) store64((u8*)s+8*(x+5*y),l)
#define XL(x,y,l) xor64((u8*)s+8*(x+5*y),l)
void KeccakF1600(void *s)
{
    ui r,x,y,i,j,Y; u8 R=0x01; u64 C[5],D;
    for(i=0; i<24; i++) {
        /*θ*/ FOR(x,5) C[x]=rL(x,0)^rL(x,1)^rL(x,2)^rL(x,3)^rL(x,4); FOR(x,5) { D=C[(x+4)%5]^ROL(C[(x+1)%5],1); FOR(y,5) XL(x,y,D); }
        /*ρπ*/ x=1; y=r=0; D=rL(x,y); FOR(j,24) { r+=j+1; Y=(2*x+3*y)%5; x=y; y=Y; C[0]=rL(x,y); wL(x,y,ROL(D,r%64)); D=C[0]; }
        /*χ*/ FOR(y,5) { FOR(x,5) C[x]=rL(x,y); FOR(x,5) wL(x,y,C[x]^((~C[(x+1)%5])&C[(x+2)%5])); }
        /*ι*/ FOR(j,7) if (LFSR86540(&R)) XL(0,0,(u64)1<<((1<<j)-1));
    }
}
void Keccak(ui r, ui c, const u8 *in, u64 inLen, u8 sfx, u8 *out, u64 outLen)
{
    /*initialize*/ u8 s[200]; ui R=r/8; ui i,b=0; FOR(i,200) s[i]=0;
    /*absorb*/ while(inLen>0) { b=(inLen<R)?inLen:R; FOR(i,b) s[i]^=in[i]; in+=b; inLen-=b; if (b==R) { KeccakF1600(s); b=0; } }
    /*pad*/ s[b]^=sfx; if((sfx&0x80)&&(b==(R-1))) KeccakF1600(s); s[R-1]^=0x80; KeccakF1600(s);
    /*squeeze*/ while(outLen>0) { b=(outLen<R)?outLen:R; FOR(i,b) out[i]=s[i]; out+=b; outLen-=b; if(outLen>0) KeccakF1600(s); }
}

#else

// derived from Keccak (KCP) readable-and-compact C implementation

/*
Implementation by the Keccak, Keyak and Ketje Teams, namely, Guido Bertoni,
Joan Daemen, Michaël Peeters, Gilles Van Assche and Ronny Van Keer, hereby
denoted as "the implementer".

For more information, feedback or questions, please refer to our websites:
http://keccak.noekeon.org/
http://keyak.noekeon.org/
http://ketje.noekeon.org/

To the extent possible under law, the implementer has waived all copyright
and related or neighboring rights to the source code in this file.
http://creativecommons.org/publicdomain/zero/1.0/
*/

/*
================================================================
The purpose of this source file is to demonstrate a readable and compact
implementation of all the Keccak instances approved in the FIPS 202 standard,
including the hash functions and the extendable-output functions (XOFs).

We focused on clarity and on source-code compactness,
rather than on the performance.

The advantages of this implementation are:
    + The source code is compact, after removing the comments, that is. :-)
    + There are no tables with arbitrary constants.
    + For clarity, the comments link the operations to the specifications using
        the same notation as much as possible.
    + There is no restriction in cryptographic features. In particular,
        the SHAKE128 and SHAKE256 XOFs can produce any output length.
    + The code does not use much RAM, as all operations are done in place.

The drawbacks of this implementation are:
    - There is no message queue. The whole message must be ready in a buffer.
    - It is not optimized for peformance.

The implementation is even simpler on a little endian platform. Just define the
LITTLE_ENDIAN symbol in that case.

For a more complete set of implementations, please refer to
the Keccak Code Package at https://github.com/gvanas/KeccakCodePackage

For more information, please refer to:
    * [Keccak Reference] http://keccak.noekeon.org/Keccak-reference-3.0.pdf
    * [Keccak Specifications Summary] http://keccak.noekeon.org/specs_summary.html

This file uses UTF-8 encoding, as some comments use Greek letters.
================================================================
*/

/**
  * Function to compute the Keccak[r, c] sponge function over a given input.
  * @param  rate            The value of the rate r.
  * @param  capacity        The value of the capacity c.
  * @param  input           Pointer to the input message.
  * @param  inputByteLen    The number of input bytes provided in the input message.
  * @param  delimitedSuffix Bits that will be automatically appended to the end
  *                         of the input message, as in domain separation.
  *                         This is a byte containing from 0 to 7 bits
  *                         These <i>n</i> bits must be in the least significant bit positions
  *                         and must be delimited with a bit 1 at position <i>n</i>
  *                         (counting from 0=LSB to 7=MSB) and followed by bits 0
  *                         from position <i>n</i>+1 to position 7.
  *                         Some examples:
  *                             - If no bits are to be appended, then @a delimitedSuffix must be 0x01.
  *                             - If the 2-bit sequence 0,1 is to be appended (as for SHA3-*), @a delimitedSuffix must be 0x06.
  *                             - If the 4-bit sequence 1,1,1,1 is to be appended (as for SHAKE*), @a delimitedSuffix must be 0x1F.
  *                             - If the 7-bit sequence 1,1,0,1,0,0,0 is to be absorbed, @a delimitedSuffix must be 0x8B.
  * @param  output          Pointer to the buffer where to store the output.
  * @param  outputByteLen   The number of output bytes desired.
  * @pre    One must have r+c=1600 and the rate a multiple of 8 bits in this implementation.
  */

/**
  *  Function to compute SHAKE128 on the input message with any output length.
  */
void
FIPS202_SHAKE128(const uint8_t *input, size_t inputByteLen, uint8_t *output, size_t outputByteLen)
{
    keccak k (1344, 256, 0x1f);
    k.update (input, inputByteLen);
    k.digest (output, outputByteLen);
}

/**
  *  Function to compute SHAKE256 on the input message with any output length.
  */
void FIPS202_SHAKE256(const uint8_t *input, size_t inputByteLen, uint8_t *output, size_t outputByteLen)
{
    keccak k (1088, 512, 0x1f);
    k.update (input, inputByteLen);
    k.digest (output, outputByteLen);
}

/**
  *  Function to compute SHA3-224 on the input message. The output length is fixed to 28 bytes.
  */
void FIPS202_SHA3_224(const uint8_t *input, size_t inputByteLen, uint8_t *output)
{
    keccak k (1152, 448, 0x06);

    for (unsigned int i = 0; i < inputByteLen; ++i)
        k.update (&input[i], 1);
    //k.update (input, inputByteLen);
    k.digest (output, 28);
}

/**
  *  Function to compute SHA3-256 on the input message. The output length is fixed to 32 bytes.
  */
void FIPS202_SHA3_256(const uint8_t *input, size_t inputByteLen, uint8_t *output)
{
    keccak k (1088, 512, 0x06);
    k.update (input, inputByteLen);
    k.digest (output, 32);
}

/**
  *  Function to compute SHA3-384 on the input message. The output length is fixed to 48 bytes.
  */
void FIPS202_SHA3_384(const uint8_t *input, size_t inputByteLen, uint8_t *output)
{
    keccak k (832, 768, 0x06);
    k.update (input, inputByteLen);
    k.digest (output, 48);
}

/**
  *  Function to compute SHA3-512 on the input message. The output length is fixed to 64 bytes.
  */
void FIPS202_SHA3_512(const uint8_t *input, size_t inputByteLen, uint8_t *output)
{
    keccak k (576, 1024, 0x06);
    k.update (input, inputByteLen);
    k.digest (output, 64);
}


/*
================================================================
A readable and compact implementation of the Keccak-f[1600] permutation.
================================================================
*/

//static constexpr
//size_t
//i (size_t x, size_t y)
//{
//    return x + 5 * y;
//}


/**
  * Function that computes the linear feedback shift register (LFSR) used to
  * define the round constants (see [Keccak Reference, Section 1.2]).
  */
class lfsr86540 {
public:
    lfsr86540  ():
        value (0x1)
    { ; }


    bool
    update (void)
    {
        bool result = value & 0x01;
        if (value & 0x80)
            // Primitive polynomial over GF(2): x^8+x^6+x^5+x^4+1
            value = (value << 1) ^ 0x71;
        else
            value <<= 1;
        return result;
    }


private:
    uint8_t value;
};


// θ step, see [Keccak Reference, Section 2.3.2]
static void
permute_theta (uint64_t m_words[5][5])
{
    uint64_t C[5], D;

    // Compute the parity of the columns
    for (unsigned x = 0; x < 5; ++x)
        C[x] = m_words[0][x] ^ m_words[1][x] ^ m_words[2][x] ^ m_words[3][x] ^ m_words[4][x];

    for (unsigned x = 0; x < 5; ++x) {
        // Compute the θ effect for a given column
        D = C[(x+4)%5] ^ rotatel (C[(x+1)%5], 1);

        // Add the θ effect to the whole column
        for (unsigned y = 0; y < 5; ++y)
            m_words[y][x] ^= D;
    }
}


void
permute_rho (uint64_t m_words[5][5])
{
    m_words[0][1] = rotatel (m_words[0][1],  1);
    m_words[0][2] = rotatel (m_words[0][2], 62);
    m_words[0][3] = rotatel (m_words[0][3], 28);
    m_words[0][4] = rotatel (m_words[0][4], 27);
    m_words[1][0] = rotatel (m_words[1][0], 36);
    m_words[1][1] = rotatel (m_words[1][1], 44);
    m_words[1][2] = rotatel (m_words[1][2],  6);
    m_words[1][3] = rotatel (m_words[1][3], 55);
    m_words[1][4] = rotatel (m_words[1][4], 20);
    m_words[2][0] = rotatel (m_words[2][0],  3);
    m_words[2][1] = rotatel (m_words[2][1], 10);
    m_words[2][2] = rotatel (m_words[2][2], 43);
    m_words[2][3] = rotatel (m_words[2][3], 25);
    m_words[2][4] = rotatel (m_words[2][4], 39);
    m_words[3][0] = rotatel (m_words[3][0], 41);
    m_words[3][1] = rotatel (m_words[3][1], 45);
    m_words[3][2] = rotatel (m_words[3][2], 15);
    m_words[3][3] = rotatel (m_words[3][3], 21);
    m_words[3][4] = rotatel (m_words[3][4],  8);
    m_words[4][0] = rotatel (m_words[4][0], 18);
    m_words[4][1] = rotatel (m_words[4][1],  2);
    m_words[4][2] = rotatel (m_words[4][2], 61);
    m_words[4][3] = rotatel (m_words[4][3], 56);
    m_words[4][4] = rotatel (m_words[4][4], 14);
    return;


    for (size_t i = 1; i < 25; ++i) {
        //unsigned r = ((t+1)*(t+2)/2)%64;
        unsigned r = ((i + 1) * (i + 2) / 2) % 64;

        m_words[i/5][i%5] = rotatel (m_words[i/5][i%5], r);
    }
}


void
permute_pi (uint64_t m_words[5][5])
{
    //auto A = reinterpret_cast<uint64_t*> (m_words);

    //uint64_t A1;
    //A1 = A[1];
    //A[ 1] = A[ 6];
    //A[ 6] = A[ 9];
    //A[ 9] = A[22];
    //A[22] = A[14];
    //A[14] = A[20];
    //A[20] = A[ 2];
    //A[ 2] = A[12];
    //A[12] = A[13];
    //A[13] = A[19];
    //A[19] = A[23];
    //A[23] = A[15];
    //A[15] = A[ 4];
    //A[ 4] = A[24];
    //A[24] = A[21];
    //A[21] = A[ 8];
    //A[ 8] = A[16];
    //A[16] = A[ 5];
    //A[ 5] = A[ 3];
    //A[ 3] = A[18];
    //A[18] = A[17];
    //A[17] = A[11];
    //A[11] = A[ 7];
    //A[ 7] = A[10];
    //A[10] = A1;
    //return;

    unsigned x = 1, y = 0;
    uint64_t current = m_words[y][x];
    uint64_t temp;
    // Iterate over ((0 1)(2 3))^t * (1 0) for 0 ≤ t ≤ 23
    for (unsigned t = 0; t < 24; ++t) {
        unsigned int Y = (2*x+3*y)%5;
        x = y;
        y = Y;

        temp = m_words[y][x];
        m_words[y][x] = current;
        current = temp;
    }

    //for (unsigned int i = 0; i < 5; ++i)
    //    for (unsigned int j = 0; j < 5; ++j)
    //        m_words[j][(2*i+3*j)%5] = m_words[i][j];
}


/**
 * Function that computes the Keccak-f[1600] permutation on the given state.
 */
void
keccak::permute (void)
{
    for (size_t i = 0; i < m_bitrate/64; ++i)
        m_words[i/5][i%5] = ltoh (m_words[i/5][i%5]);

    lfsr86540 shift;

    for (unsigned round = 0; round < 24; ++round) {
        permute_theta (m_words);
        permute_rho (m_words);
        permute_pi (m_words);
        

        if (0) {   // === ρ and π steps (see [Keccak Reference, Sections 2.3.3 and 2.3.4]) ===
            uint64_t current, temp;
            // Start at coordinates (1 0)
            unsigned x = 1, y = 0;
            current = m_words[y][x];
            // Iterate over ((0 1)(2 3))^t * (1 0) for 0 ≤ t ≤ 23
            for (unsigned t = 0; t < 24; ++t) {
                // Compute the rotation constant r = (t+1)(t+2)/2
                unsigned int r = ((t+1)*(t+2)/2)%64;

                // Compute ((0 1)(2 3)) * (x y)
                unsigned int Y = (2*x+3*y)%5; x = y; y = Y;

                // Swap current and state(x,y), and rotate
                temp = m_words[y][x];
                m_words[y][x] = rotatel (current, r);
                current = temp;
            }
        }

        {   // === χ step (see [Keccak Reference, Section 2.3.1]) ===
            uint64_t temp[5];
            for (unsigned y = 0; y < 5; ++y) {
                // Take a copy of the plane
                for (unsigned x = 0; x < 5; ++x)
                    temp[x] = m_words[y][x];

                // Compute χ on the plane
                for(unsigned x = 0; x < 5; ++x)
                    m_words[y][x] = temp[x] ^((~temp[(x+1)%5]) & temp[(x+2)%5]);
            }
        }

        {   // === ι step (see [Keccak Reference, Section 2.3.5]) ===
            for (unsigned j = 0; j < 7; ++j) {
                unsigned int bitPosition = (1 << j) - 1; //2^j-1
                if (shift.update ())
                    m_words[0][0] ^= uint64_t{1} << bitPosition;
            }
        }
    }
}

/*
================================================================
A readable and compact implementation of the Keccak sponge functions
that use the Keccak-f[1600] permutation.
================================================================
*/

#include <cstring>


void
keccak::update (
    const uint8_t *input,
    size_t len
) { 
    unsigned int byterate = m_bitrate / 8;

    while (len) {
        auto chunk = util::min (len, byterate - m_cursor);

        for (unsigned i = 0; i < chunk; ++i)
            m_bytes[m_cursor++] ^= *input++;

        len -= chunk;

        if (m_cursor == byterate) {
            permute ();
            m_cursor = 0;
        }
    }
}


void
keccak::digest (
    uint8_t *output,
    size_t len
) {
    unsigned byterate = m_bitrate / 8u;

    // === Do the padding and switch to the squeezing phase ===
    // Absorb the last few bits and add the first bit of padding (which
    // coincides with the delimiter in delimitedSuffix)
    m_bytes[m_cursor] ^= m_suffix;

    // If the first bit of padding is at position rate-1, we need a whole new
    // block for the second bit of padding
    if (m_suffix & 0x80 && m_cursor == byterate - 1)
        permute ();

    // Add the second bit of padding
    m_bytes[byterate - 1] ^= 0x80;

    // === Squeeze out all the output blocks ===
    while (len) {
        permute ();

        auto chunk = util::min (len, byterate);
        std::copy_n (m_bytes.begin (), chunk, output);

        output += chunk;
        len    -= chunk;
    }
}


keccak::keccak (unsigned _bitrate,
                unsigned _capacity,
                uint8_t  _suffix):
    m_bitrate (_bitrate),
    m_capacity (_capacity),
    m_suffix (_suffix),
    m_cursor (0)
{
    // we could support bitrates that are multiples of 8, but 64 simplifies
    // some state handling, and the SHA-3 constants are all multiples of 64
    // bits anyway.
    if ((m_bitrate + m_capacity) / 8 != sizeof (m_bytes) || m_bitrate % 64 != 0)
        throw "error";

    std::fill (std::begin (m_bytes), std::end (m_bytes), 0);
}



#endif
