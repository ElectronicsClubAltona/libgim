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


#ifndef __UTIL_VIEW_HPP
#define __UTIL_VIEW_HPP

#include <cstdlib>
#include <iostream>

namespace util {
    template <typename T>
    struct view {
    public:
        using value_type = typename std::iterator_traits<T>::value_type;

        view (T first, T last);

        T begin (void);
        T end   (void);

        bool empty (void) const;
        size_t size (void) const;

        value_type& operator[] (size_t);
        const value_type& operator[] (size_t) const;

        bool operator== (view) const;

    private:
        T m_begin;
        T m_end;
    };

    template <typename T>
    std::ostream& operator<< (std::ostream&, view<T>);
}

#include "./view.ipp"

#endif
