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
 *      2014-2016, Danny Robson <danny@nerdcruft.net>
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>

#include <cstdlib>
#include <cstring>

#include "io.hpp"

#include "hash/simple.hpp"

#include "hash/adler.hpp"
#include "hash/bsdsum.cpp"
#include "hash/crc.hpp"
#include "hash/fletcher.hpp"
#include "hash/md2.hpp"
#include "hash/md4.hpp"
#include "hash/md5.hpp"
#include "hash/ripemd.hpp"
#include "hash/sha1.hpp"
#include "hash/sha2.hpp"


///////////////////////////////////////////////////////////////////////////////
static
const char* NAMES[] = {
    "adler32",
    "bsdsum",
    "crc32",
    "fletcher"
    "MD2",
    "MD4",
    "MD5",
    "RIPEMD",
    "SHA1",
    "SHA256",
};


///////////////////////////////////////////////////////////////////////////////
std::ostream&
print_digest (std::ostream &os, uint32_t t)
{
    return os << std::hex << t << std::dec;
}


//-----------------------------------------------------------------------------
template <size_t S>
std::ostream&
print_digest (std::ostream &os, std::array<uint8_t,S> digest)
{
    os << std::hex;

    for (auto c: digest)
        os << +(c >> 4) << +(c & 0x0F);

    os << std::dec;
    return os;
}


///////////////////////////////////////////////////////////////////////////////
static
void
compute (const std::string &name,
         const unsigned char *restrict first,
         const unsigned char *restrict last)
{
    #define stream(TYPE) do {                                       \
        if (name != #TYPE)                                          \
            break;                                                  \
                                                                    \
        auto sum = util::hash::simple<util::hash::TYPE> (           \
            first, last                                             \
        );                                                          \
                                                                    \
        print_digest (std::cout, sum) << '\n';                      \
        return;                                                     \
    } while (0);

        stream (adler32);
        stream (bsdsum);
        stream (crc32);

        stream (MD2);
        stream (MD4);
        stream (MD5);
        stream (RIPEMD);
        stream (SHA1);
        stream (SHA256);

    #undef stream
}


///////////////////////////////////////////////////////////////////////////////
enum {
    ARG_CMD,
    ARG_HASH,
    ARG_INPUT,

    NUM_ARGS
};


//-----------------------------------------------------------------------------
void
print_usage (int argc, char **argv)
{
    (void)argc;

    std::cerr << argv[ARG_CMD] << " [";
    for (auto name: NAMES)
        std::cerr << name << "|";
    std::cerr << "] <input>\n";
}


//-----------------------------------------------------------------------------
int
main (int argc, char **argv)
{
    if (argc != NUM_ARGS) {
        print_usage (argc, argv);
        return EXIT_FAILURE;
    }

    if (strcmp (argv[ARG_INPUT], "-")) {
        util::mapped_file src (argv[ARG_INPUT]);
        compute (argv[ARG_HASH], src.cbegin (), src.cend ());

        return EXIT_SUCCESS;
    } else {
        //compute (argv[ARG_HASH], std::cin);
        return EXIT_SUCCESS;
    }
}
