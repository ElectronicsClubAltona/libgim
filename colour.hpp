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
    struct colour : public coord::base<S,T,colour,coord::rgba> {
        using coord::base<S,T,util::colour,coord::rgba>::base;
        using base_t = coord::base<S,T,util::colour,coord::rgba>;

        template <typename U>
        colour<S,U>
        cast (void) const;

        static const colour WHITE;
        static const colour BLACK;
        static const colour RED;
        static const colour BLUE;
        static const colour GREEN;
    };

    // Convenience types
    typedef colour<4,float> colour4f;

    // Serialisation
    const json::tree::node& operator>> (const json::tree::node&, util::colour4f&);

    template <size_t S, typename T>
    std::ostream& operator<< (std::ostream&, util::colour<S,T>);
}

#include "colour.ipp"

#endif
