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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef CRUFT_UTIL_COORD_INIT_HPP
#define CRUFT_UTIL_COORD_INIT_HPP

#include "store.hpp"

#include <cstddef>

namespace util::coord {
    template <size_t S, typename T, typename SelfT>
    struct init;

    //-------------------------------------------------------------------------
    template <typename T, typename SelfT>
    struct init<1,T,SelfT> : public store<1,T,SelfT>
    {
        using store<1,T,SelfT>::store;
        constexpr init () = default;
        constexpr init (T v0):
            store<1,T,SelfT> ({v0})
        { ; }
    };


    //-------------------------------------------------------------------------
    template <typename T, typename SelfT>
    struct init<2,T,SelfT> : public store<2,T,SelfT>
    {
        using store<2,T,SelfT>::store;
        constexpr init () = default;
        constexpr init (T v0, T v1):
            store<2,T,SelfT> ({ v0, v1 })
        { ; }
    };


    //-------------------------------------------------------------------------
    template <typename T, typename SelfT>
    struct init<3,T,SelfT> : public store<3,T,SelfT>
    {
        using store<3,T,SelfT>::store;
        constexpr init () = default;
        constexpr init (T v0, T v1, T v2):
            store<3,T,SelfT> ({v0, v1, v2})
        { ; }
    };


    //-------------------------------------------------------------------------
    template <typename T, typename SelfT>
    struct init<4,T,SelfT> : public store<4,T,SelfT>
    {
        using store<4,T,SelfT>::store;
        constexpr init () = default;
        constexpr init (T v0, T v1, T v2, T v3):
            store<4,T,SelfT> ({ v0, v1, v2, v3 })
        { ; }
    };
}

#endif
