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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */


#include "json/except.hpp"
#include "json/tree.hpp"
#include "json/schema.hpp"
#include "json/except.hpp"

#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

enum {
    ARG_CMD,
    ARG_SCHEMA,
    ARG_INPUT,

    NUM_ARGS
};


int
main (int argc, char **argv) {
    if (argc != NUM_ARGS) {
        std::cerr << argv[ARG_CMD] << " <schema> <json>\n";
        return EXIT_FAILURE;
    }

    auto schema = json::tree::parse (fs::path (argv[ARG_SCHEMA]));
    auto input  = json::tree::parse (fs::path (argv[ARG_INPUT]));

    try {
        json::schema::validate (*input, schema->as_object ());
    } catch (const json::error &e) {
        std::cerr << "error: " << e.what () << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
