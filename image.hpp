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
 * Copyright 2011-2014 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_IMAGE_HPP
#define __UTIL_IMAGE_HPP

#include <cstdint>
#include <cstdlib>
#include <boost/filesystem/path.hpp>

namespace util {
    struct pgm {
        static void write (const uint8_t *restrict pixels,
                           size_t width,
                           size_t height,
                           size_t stride,
                           const boost::filesystem::path &path);
    };

    struct ppm {
        static void write (const uint8_t *restrict pixels,
                           size_t width,
                           size_t height,
                           size_t stride,
                           const boost::filesystem::path &path);
    };
}

#endif
