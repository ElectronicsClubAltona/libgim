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
#include "introspection.hpp"

#include <ostream>

namespace util {
    /// An RGBA colour POD type.
    template <size_t S, typename T>
    struct colour : public coord::base<S,T,colour,coord::rgba,coord::hsv> {
        using coord::base<S,T,util::colour,coord::rgba,coord::hsv>::base;
        using base_t = coord::base<S,T,util::colour,coord::rgba,coord::hsv>;

        // overloaded cast operator which assumes values are unit normalised
        template <typename U>
        colour<S,U>
        cast (void) const;

        /// parse colours specified as "#AABBCCDD".
        ///
        /// * the leading hash is optional.
        /// * all components must be 2 hex digits.
        static colour parse_html (const char*);
        static colour parse_html (const std::string&);

        /// look up the name of a colour from those specified in
        /// html/x11/etc specifications.
        static colour from_html (const std::string &name);
        static colour from_x11  (const std::string &name);

        /// look up all the specifications and returns the colour from one
        /// that matches. the search order is unspecified, so if you want a
        /// known colour then try them first yourself.
        static colour from_string (const std::string &name);
    };

    // Convenience types
    template <typename T> using colour1 = colour<1,T>;
    template <typename T> using colour3 = colour<3,T>;
    template <typename T> using colour4 = colour<4,T>;

    typedef colour1<uint8_t> colour1u;
    typedef colour3<uint8_t> colour3u;
    typedef colour4<uint8_t> colour4u;

    typedef colour1<float> colour1f;
    typedef colour3<float> colour3f;
    typedef colour4<float> colour4f;

    // RGB/HSV conversions
    colour3f rgb_to_hsv (colour3f);
    colour3f hsv_to_rgb (colour3f);

    // ostream/istream operators
    template <size_t S, typename T>
    std::ostream&
    operator<< (std::ostream&, util::colour<S,T>);

    template <size_t S, typename T>
    std::istream&
    operator>> (std::istream&, util::colour<S,T>&);

    // type name introspection specialisation
    template <size_t S, typename T>
    struct type_name<colour<S,T>> {
        static constexpr const char value[] = "colour";
    };
}

#include "colour.ipp"

#endif
