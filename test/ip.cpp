
#include "../ip.hpp"

#include "../debug.hpp"
#include "../platform.hpp"
#include "../types.hpp"

#include <cstdlib>

#if defined(PLATFORM_WIN32)
    #include <ws2tcpip.h>
#else
    #include <arpa/inet.h>
#endif


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

        uint32_t mine = *(uint32_t*)(parsed.m_octets);

        struct addrinfo *theirs;

        int err = getaddrinfo (data[i].str, nullptr, nullptr, &theirs);
        CHECK_EQ (err, 0);

        CHECK_EQ (mine, reinterpret_cast<sockaddr_in*> (theirs)->sin_addr.s_addr);

        //CHECK_HARD (inet_pton (AF_INET, data[i].str, &theirs) == 1);
        //CHECK_HARD (theirs == mine);
    }

    return EXIT_SUCCESS;
}
