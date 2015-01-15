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

#include "detail/coord.hpp"

#include "json.hpp"

#include <iostream>

namespace util {
    /// An RGBA colour POD type.
    template <typename T>
    struct colour : public detail::coord<4, T> {
        using detail::coord<4,T>::coord;

        static const colour WHITE;
        static const colour BLACK;
        static const colour RED;
        static const colour BLUE;
        static const colour GREEN;
    };

    typedef colour<float> colour4f;

    const json::node& operator>> (const json::node&, util::colour4f&);
    std::ostream& operator<< (std::ostream&, const util::colour4f&);
}

#endif
