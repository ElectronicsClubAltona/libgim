/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2010-2012 Danny Robson <danny@nerdcruft.net>
 */


#include "exe.hpp"

#include "platform.hpp"

#ifdef PLATFORM_LINUX

#include "types.hpp"
#include "except.hpp"

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <unistd.h>
#include <stdexcept>
#include <vector>

boost::filesystem::path
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

        return boost::filesystem::path (resolved.data (), resolved.data () + written);
}

#else
    #error "Unknown platform"
#endif
