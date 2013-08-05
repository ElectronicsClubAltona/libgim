
#include "../ip.hpp"

#include "../debug.hpp"
#include "../platform.hpp"
#include "../types.hpp"

#include <cstdlib>

using namespace std;


int
main (int, char **) {
    struct ip_test {
        const char     *str;
        const ipv4::ip  ip;
    } data [] = {
        {         "0.0.0.0", {   0,   0,   0,   0 } },
        { "255.255.255.255", { 255, 255, 255, 255 } },
        {       "127.0.0.1", { 127,   0,   0,   1 } }
    };

    for (unsigned int i = 0; i < elems (data); ++i) {
        ipv4::ip parsed (ipv4::ip::parse (data[i].str));
        CHECK_HARD (parsed == data[i].ip);
    }

    return EXIT_SUCCESS;
}
