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

#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>

#include <cstdlib>
#include <cstring>

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


static const char* NAMES[] = {
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


static void
compute (std::istream &is, const char *name) {
    std::vector<uint8_t> data {
        std::istream_iterator<uint8_t> (is),
        std::istream_iterator<uint8_t> ( )
    };

    #define simple(FUNC) do {                                                   \
        if (std::strcmp (name, #FUNC))                                          \
            break;                                                              \
                                                                                \
        std::cout << std::hex << FUNC (data.data (), data.size ()) << "\n";     \
    } while (0)

        simple (adler32);
        simple (bsdsum);
        simple (crc32);

    #undef simple

    #define stream(TYPE) do {                                       \
        if (std::strcmp (name, #TYPE))                              \
            break;                                                  \
                                                                    \
        util::hash::TYPE accum;                                     \
        accum.update (data.data (), data.size ());                  \
        accum.finish ();                                            \
                                                                    \
        std::cout << std::hex;                                      \
        for (auto c: accum.digest ())                               \
            std::cout << unsigned (c >> 4) << unsigned(c & 0x0F);   \
        std::cout << std::dec << "\n";                              \
        return;                                                     \
    } while (0);

        stream (MD2);
        stream (MD4);
        stream (MD5);
        stream (RIPEMD);
        stream (SHA1);
        stream (SHA256);

    #undef stream
}


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
main (int argc, char **argv) {
    if (argc != NUM_ARGS) {
        print_usage (argc, argv);
        return EXIT_FAILURE;
    }

    if (strcmp (argv[ARG_INPUT], "-")) {
        std::cerr << "stdin only currently\n";
        return EXIT_FAILURE;
    }

    compute (std::cin, argv[ARG_HASH]);
    return EXIT_SUCCESS;
}
