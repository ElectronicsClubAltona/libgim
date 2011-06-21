
#include "../types.hpp"

#include "../debug.hpp"

#include <cstdlib>
#include <arpa/inet.h>
#include <netinet/in.h>

using namespace std;


int 
main (int argc, char **argv) {

    uint16_t u16 = 0x1358;
    uint32_t u32 = 0x12345678;

    check_eq (htons (u16), hton (u16));
    check_eq (htonl (u32), hton (u32));

    check_eq (ntohs (u16), hton (u16));
    check_eq (ntohl (u32), hton (u32));

    return EXIT_SUCCESS;
}
