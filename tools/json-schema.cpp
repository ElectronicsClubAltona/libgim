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
