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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */


#ifndef __UTIL_VIEW_HPP
#define __UTIL_VIEW_HPP

#include <cstdlib>
#include <iostream>

namespace util {
    class view {
    public:
        view ();
        view (const char *str);
        view (const char *first, const char *last);

        const char *begin (void) const;
        const char *end   (void) const;

        bool empty () const;
        size_t size (void) const;
        const char& operator[] (size_t) const;

    private:
        const char *m_begin;
        const char *m_end;
    };


    std::ostream& operator<< (std::ostream&, view);
}
#endif
