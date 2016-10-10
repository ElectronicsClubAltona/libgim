
#include "except.hpp"

#include <windows.h>

std::experimental::filesystem::path
util::image_path (void) {
        std::vector<char> resolved (256);

retry:
        const auto written = GetModuleFileName (nullptr, resolved.data (), resolved.size ());
        if (written == 0)
            win32_error::throw_code ();

        if (written == resolved.size ()) {
            resolved.resize (resolved.size () * 2);
            goto retry;
        }

        return std::experimental::filesystem::path (resolved.data (), resolved.data () + written);
}
