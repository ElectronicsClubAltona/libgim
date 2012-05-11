
#include "../ip.hpp"

#include "../debug.hpp"
#include "../types.hpp"

#include <arpa/inet.h>
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

        uint32_t mine = *(uint32_t*)(parsed.m_octets),
                 theirs;
        CHECK_HARD (inet_pton (AF_INET, data[i].str, &theirs) == 1);
        CHECK_HARD (theirs == mine);
    }

    return EXIT_SUCCESS;
}
