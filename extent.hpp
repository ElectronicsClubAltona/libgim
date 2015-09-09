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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_EXTENT_HPP
#define __UTIL_EXTENT_HPP

#include "coord.hpp"
#include "vector.hpp"
#include <iostream>


namespace util {
    /**
     * A pure two-dimensional size, without positioning
     */
    template <size_t S, typename T>
    struct extent : public coord::base<S,T,extent,coord::whd>
    {
        using coord::base<S,T,util::extent,coord::whd>::base;

        extent () = default;
        explicit extent (vector<S,T>);

        T    area  (void) const;
        T diameter (void) const;

        template <typename U = float>
        U aspect (void) const;

        extent expanded (vector<S,T>) const;
        extent expanded (T) const;
        extent contracted (vector<S,T>) const;
        extent contracted (T) const;

        bool empty (void) const;

        static const extent MAX;
        static const extent MIN;
    };

    // convenience typedefs
    template <typename T> using extent2 = extent<2,T>;
    template <typename T> using extent3 = extent<3,T>;

    typedef extent2<intmax_t> extent2i;
    typedef extent2<size_t> extent2u;
    typedef extent2<float> extent2f;
    typedef extent2<double> extent2d;

    typedef extent3<size_t> extent3u;
    typedef extent3<float> extent3f;

    template <size_t S, typename T>
    std::ostream& operator<< (std::ostream&, util::extent<S,T>);
}


#include "extent.ipp"

#endif
