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
