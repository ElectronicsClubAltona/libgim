/*
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Copyright 2010-2012 Danny Robson <danny@nerdcruft.net>
 */


#include "exe.hpp"

#include "platform.hpp"

#if defined(PLATFORM_LINUX)

#include "except.hpp"
#include "cast.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <unistd.h>
#include <stdexcept>
#include <vector>

std::experimental::filesystem::path
util::image_path (void) {
        static const char PROC_SELF[] = "/proc/self/exe";

        // We can't use lstat to check the size of the link in proc, as Linux
        // will return 0 for most entries under proc. Instead we've got to
        // iterate for a correct size.
        std::vector<char> resolved (256);

retry:
        const auto written = readlink (PROC_SELF, resolved.data (), resolved.size ());
        if (written < 0)
            errno_error::throw_code ();

        if (sign_cast<size_t> (written) == resolved.size ()) {
            resolved.resize (resolved.size () * 2);
            goto retry;
        }

        return std::experimental::filesystem::path (resolved.data (), resolved.data () + written);
}

#elif defined(PLATFORM_FREEBSD)

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

#elif defined(PLATFORM_WIN32)

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

#else
    #error "Unknown platform"

// Mac OS X: _NSGetExecutablePath() (man 3 dyld)
// Linux: readlink /proc/self/exe
// Solaris: getexecname()
// FreeBSD: sysctl CTL_KERN KERN_PROC KERN_PROC_PATHNAME -1
// BSD with procfs: readlink /proc/curproc/file
// Windows: GetModuleFileName() with hModule = NULL

#endif
