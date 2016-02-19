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
 * Copyright 2011-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_NETPBM_HPP
#define __UTIL_NETPBM_HPP

#include "./image/buffer.hpp"

#include <cstdint>
#include <cstddef>
#include <ostream>
#include <boost/filesystem/path.hpp>

namespace util {
    // Portable GrayMap: single component greyscale.
    struct pgm {
        static image::buffer<1,uint8_t> read (const boost::filesystem::path&);

        static void write (const image::buffer<1,uint8_t> &src,
                           const boost::filesystem::path &dst);
        static void write (const image::buffer<1,uint8_t> &src,
                           std::ostream &dst);

        static void write (const uint8_t *restrict pixels,
                           size_t width,
                           size_t height,
                           size_t stride,
                           std::ostream &dst);
        static void write (const uint8_t *restrict pixels,
                           size_t width,
                           size_t height,
                           size_t stride,
                           const boost::filesystem::path &path);
    };

    /// Portable PixMap: 3-component colour images.
    struct ppm {
        static void write (const uint8_t *restrict pixels,
                           size_t width,
                           size_t height,
                           size_t stride,
                           const boost::filesystem::path &path);
        static void write (const uint8_t *restrict pixels,
                           size_t width,
                           size_t height,
                           size_t stride,
                           std::ostream &dst);
    };
}

#endif
