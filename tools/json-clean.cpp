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
 * Copyright 2012-2016 Danny Robson <danny@nerdcruft.net>
 */

#include "io.hpp"
#include "json/except.hpp"
#include "json/tree.hpp"

#include <cstdlib>
#include <iostream>


///////////////////////////////////////////////////////////////////////////////
enum {
    ARG_COMMAND,
    ARG_INPUT,

    NUM_ARGS
};


//-----------------------------------------------------------------------------
void
print_usage (int argc, char **argv)
{
    (void)argc;
    std::cerr << "usage: " << argv[0] << " <input>\n";
}


//-----------------------------------------------------------------------------
int
main (int argc, char **argv)
{
    if (argc != NUM_ARGS) {
        print_usage (argc, argv);
        return EXIT_FAILURE;
    }

    try {
        const util::mapped_file src (argv[ARG_INPUT]);
        std::cout << *json::tree::parse (util::view{src}.cast<const char> ()) << '\n';
    } catch (const json::error& err) {
        std::cerr << err.what () << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
