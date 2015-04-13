
#include "endian.hpp"

#include "debug.hpp"
#include "tap.hpp"
#include "platform.hpp"

#include <cstdlib>
#if defined(PLATFORM_WIN32)
    #include <winsock2.h>
#else
    #include <arpa/inet.h>
    #include <netinet/in.h>
#endif


int
main (int, char **) {

    uint16_t u16 = 0x1358;
    uint32_t u32 = 0x12345678;

    CHECK_EQ (htons (u16), hton (u16));
    CHECK_EQ (htonl (u32), hton (u32));

    CHECK_EQ (ntohs (u16), hton (u16));
    CHECK_EQ (ntohl (u32), hton (u32));

    util::TAP::logger tap;
    tap.todo ("convert to TAP");
}
