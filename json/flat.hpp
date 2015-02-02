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

#ifndef __UTIL_JSON_FLAT_HPP
#define __UTIL_JSON_FLAT_HPP

#include <boost/filesystem/path.hpp>
#include <iostream>

namespace json { namespace flat {
    enum class type {
        UNKNOWN,

        NUL,
        BOOLEAN,
        STRING,
        INTEGER,
        REAL,

        OBJECT_BEGIN,
        OBJECT_END,

        ARRAY_BEGIN,
        ARRAY_END
    };

    struct item {
        type tag;
        const char *first;
        const char *last;

        template <typename T>
        T as (void) const;
    };

    std::vector<item> parse (const char *first, const char *last);
    std::vector<item> parse (const char *first);
    std::vector<item> parse (const boost::filesystem::path&);

    std::ostream& operator<< (std::ostream&, type);
} }

#endif
