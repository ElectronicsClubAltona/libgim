/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 * 
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */

#include "../json.hpp"

#include <cstdlib>
#include <iostream>

#include <boost/filesystem/path.hpp>

enum {
    ARG_COMMAND,
    ARG_INPUT,

    NUM_ARGS
};


void
print_usage (int argc, char **argv) {
    (void)argc;
    std::cerr << "usage: " << argv[0] << " <input>\n";
}


int
main (int argc, char **argv) {
    if (argc != NUM_ARGS) {
        print_usage (argc, argv);
        return EXIT_FAILURE;
    }

    try {
        boost::filesystem::path input (argv[ARG_INPUT]);
        std::cout << *json::parse (input) << "\n";
    } catch (const json::error& err) {
        std::cerr << err.what () << "\n";
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
