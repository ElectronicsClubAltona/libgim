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
 *      2014-2015, Danny Robson <danny@nerdcruft.net>
 */

#include "json/flat.hpp"

#include <iostream>
#include <cstdlib>
#include <boost/filesystem.hpp>


enum {
    ARG_CMD,
    ARG_PATH,

    NUM_ARGS
};


int
main (int argc, char ** argv) {
    if (argc != NUM_ARGS) {
        std::cerr << "Invalid arguments. "
                  << argv[ARG_CMD] << " <path> "
                  << std::endl;
        return EXIT_FAILURE;
    }

    try {
       json::flat::parse (boost::filesystem::path (argv[ARG_PATH]));
    } catch (std::exception &x) {
        std::cerr << "Error: " << x.what () << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
