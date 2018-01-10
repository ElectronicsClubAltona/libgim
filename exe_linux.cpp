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
 * Copyright 2010-2016 Danny Robson <danny@nerdcruft.net>
 */

#include "exe.hpp"

#include "cast.hpp"
#include "posix/except.hpp"

#include <vector>
#include <experimental/filesystem>

#include <unistd.h>


///////////////////////////////////////////////////////////////////////////////
std::experimental::filesystem::path
util::image_path (void)
{
    static const char PROC_SELF[] = "/proc/self/exe";

    // We can't use lstat to check the size of the link in proc, as Linux
    // will return 0 for most entries under proc. Instead we've got to
    // iterate for a correct size.
    std::vector<char> resolved (256);

retry:
    const auto written = readlink (PROC_SELF, resolved.data (), resolved.size ());
    if (written < 0)
        posix::error::throw_code ();

    if (sign_cast<size_t> (written) == resolved.size ()) {
        resolved.resize (resolved.size () * 2);
        goto retry;
    }

    return std::experimental::filesystem::path (resolved.data (), resolved.data () + written);
}

