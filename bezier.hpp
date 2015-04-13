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

#ifndef __UTIL_BEZIER_HPP
#define __UTIL_BEZIER_HPP

#include "point.hpp"
#include "region.hpp"

#include <iostream>

namespace util {
    template <size_t S>
    class bezier {
    public:
        bezier (const util::point2f (&)[S+1]);

        point2f eval (float t) const;

        // Calculate the expanded polynomial coeffecients in terms of t
        std::array<vector2f,S+1>
        coeffs (void) const;

        size_t intersections (point2f from, point2f to) const;

        float distance (point2f) const;

        region2f region (void) const;

        point2f& operator[] (size_t idx);
        const point2f& operator[] (size_t idx) const;

    private:
        // HACK: allow easy access to component-wise arithmetic using
        // vector2f rather than point2f in the implementation.
        union {
            point2f  m_points[S+1];
            vector2f m_coeffs[S+1];
        };
    };

    template <size_t S>
    std::ostream& operator<< (std::ostream&, const bezier<S>&);
}

#endif
