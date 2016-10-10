
#include "types.hpp"
#include "except.hpp"

#include <sys/types.h>
#include <sys/sysctl.h>

std::experimental::filesystem::path
util::image_path (void)
{
    int name[] = {
        CTL_KERN,
        KERN_PROC,
        KERN_PROC_PATHNAME,
        -1
    };

    char data[1024];
    size_t len = sizeof (data);

    auto err = sysctl (name, elems (name), data, &len, nullptr, 0);
    errno_error::try_code (err);

    return std::experimental::filesystem::path (std::cbegin (data), std::cbegin (data) + len);
}
