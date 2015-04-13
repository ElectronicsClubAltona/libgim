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


#ifndef __UTIL_AABB_HPP
#define __UTIL_AABB_HPP

#include "point.hpp"
#include "extent.hpp"

#include <cstdint>
#include <iostream>

namespace util {
    template <size_t S, typename T>
    struct AABB {
        AABB () = default;
        AABB (point<S,T>, point<S,T>);

        extent<S,T> magnitude (void) const;

        bool overlaps (point<S,T>) const;

        point<S,T> closest (point<S,T>) const;

        AABB<S,T>& expand (util::vector<S,T>);
        AABB<S,T>& expand (T);
        AABB<S,T>  expanded (util::vector<S,T>);
        AABB<S,T>  expanded (T);

        AABB<S,T>& contract (util::vector<S,T>);
        AABB<S,T>& contract (T);
        AABB<S,T>  contracted (util::vector<S,T>) const;
        AABB<S,T>  contracted (T) const;

        void cover (point<S,T>);

        AABB<S,T> operator+ (vector<S,T>) const;
        AABB<S,T> operator- (vector<S,T>) const;

        bool operator== (AABB) const;

        point<S,T> p0;
        point<S,T> p1;
    };

    typedef AABB<2,float> AABB2f;
    typedef AABB<2,size_t> AABB2u;
    typedef AABB<2,intmax_t> AABB2i;

    typedef AABB<3,float> AABB3f;
    typedef AABB<3,size_t> AABB3u;
    typedef AABB<3,intmax_t> AABB3i;

    template <size_t S, typename T>
    std::ostream& operator<< (std::ostream&, AABB<S,T>);
}

#endif
