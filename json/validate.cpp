#include "../json.hpp"

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
       json::parse (boost::filesystem::path (argv[ARG_PATH]));
    } catch (json::error &x) {
        std::cerr << "Error: " << x.what () << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
