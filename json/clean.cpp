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
