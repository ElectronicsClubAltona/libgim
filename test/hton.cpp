
#include "endian.hpp"

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
main (void)
{
    util::TAP::logger tap;

    uint16_t u16 = 0x1358;
    uint32_t u32 = 0x12345678;

    tap.expect_eq (htons (u16), hton (u16), "htons");
    tap.expect_eq (htonl (u32), hton (u32), "htonl");

    tap.expect_eq (ntohs (u16), hton (u16), "ntohs");
    tap.expect_eq (ntohl (u32), hton (u32), "ntohl");

    return tap.status ();
}
