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

#ifndef __UTIL_COORD_INIT_HPP
#define __UTIL_COORD_INIT_HPP

#include "store.hpp"

#include <cstdlib>

namespace util { namespace coord {
    template <size_t S, typename T, typename...>
    struct init;

    //-------------------------------------------------------------------------
    template <typename T, typename ...tags>
    struct init<1,T,tags...> : public store<1,T,tags...>
    {
        using store<1,T,tags...>::store;
        init () = default;
        init (T v0):
            store<1,T,tags...> ({v0})
        { ; }
    };


    //-------------------------------------------------------------------------
    template <typename T, typename ...tags>
    struct init<2,T,tags...> : public store<2,T,tags...>
    {
        using store<2,T,tags...>::store;
        init () = default;
        init (T v0, T v1):
            store<2,T,tags...> ({ v0, v1 })
        { ; }
    };


    //-------------------------------------------------------------------------
    template <typename T, typename ...tags>
    struct init<3,T,tags...> : public store<3,T,tags...>
    {
        using store<3,T,tags...>::store;
        init () = default;
        init (T v0, T v1, T v2):
            store<3,T,tags...> ({v0, v1, v2})
        { ; }
    };


    //-------------------------------------------------------------------------
    template <typename T, typename ...tags>
    struct init<4,T,tags...> : public store<4,T,tags...>
    {
        using store<4,T,tags...>::store;
        init () = default;
        init (T v0, T v1, T v2, T v3):
            store<4,T,tags...> ({ v0, v1, v2, v3 })
        { ; }
    };
} }

#endif
