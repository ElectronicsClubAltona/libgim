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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_COLOUR_HPP
#define __UTIL_COLOUR_HPP

#include "coord.hpp"

#include "json/tree.hpp"

#include <iostream>

namespace util {
    /// An RGBA colour POD type.
    template <size_t S, typename T>
    struct colour : public coord::base<S,T,colour,coord::rgba,coord::hsv> {
        using coord::base<S,T,util::colour,coord::rgba,coord::hsv>::base;
        using base_t = coord::base<S,T,util::colour,coord::rgba,coord::hsv>;

        template <typename U>
        colour<S,U>
        cast (void) const;

        static const colour WHITE;
        static const colour BLACK;
        static const colour RED;
        static const colour BLUE;
        static const colour GREEN;

        static colour from_html (const std::string&);
        static colour from_x11  (const std::string&);
        static colour from_string (const std::string&);
    };

    // Convenience types
    template <typename T> using colour3 = colour<3,T>;
    template <typename T> using colour4 = colour<4,T>;

    typedef colour3<uint8_t> colour3u;
    typedef colour4<uint8_t> colour4u;

    typedef colour4<float> colour4f;
    typedef colour3<float> colour3f;

    // RGB <-> HSV
    colour3f rgb_to_hsv (colour3f);
    colour3f hsv_to_rgb (colour3f);

    // Serialisation
    const json::tree::node& operator>> (const json::tree::node&, util::colour4f&);

    template <size_t S, typename T>
    std::ostream& operator<< (std::ostream&, util::colour<S,T>);
}

#include "colour.ipp"

#endif
