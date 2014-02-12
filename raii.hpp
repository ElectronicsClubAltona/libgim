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
 * Copyright 2012 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_RAII_HPP
#define __UTIL_RAII_HPP

#include "preprocessor.hpp"

/// Defines a translation-unit-unique variable useful for unnamed scoped variables
#define raii PASTE(__unique_, __COUNTER__)

namespace util {
    /// Increments a counter for the lifetime of the object
    template <typename T>
    struct scoped_counter {
        scoped_counter (T &_counter):
            counter (_counter)
        { ++counter; }

        ~scoped_counter ()
        { --counter; }

        T &counter;
    };

    
    /// Executes a function upon object destruction
    template <typename T>
    struct scoped_function {
        scoped_function (T &&_func):
            func  (std::move (_func))
        { ; }

        ~scoped_function ()
            { func (); }

        T func;
    };
}

#endif
