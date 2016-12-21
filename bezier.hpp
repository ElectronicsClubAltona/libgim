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
 * Copyright 2015-2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_BEZIER_HPP
#define __UTIL_BEZIER_HPP

#include "point.hpp"
#include "region.hpp"

#include <ostream>

namespace util {
    struct sdot_t {
        float distance;
        float dot;
    };

    template <size_t N>
    class bezier {
    public:
        using value_type = point2f::value_type;

        bezier (const util::point2f (&)[N+1]);

        point2f eval (float t) const;

        // Calculate the expanded polynomial coeffecients in terms of t
        std::array<vector2f,N+1>
        coeffs (void) const;

        size_t intersections (point2f from, point2f to) const;

        util::vector2f tangent (float t) const;
        // 1st derivative w.r.t. t
        util::vector2f d1 (float t) const noexcept;
        // 2nd derivative w.r.t. t
        util::vector2f d2 (float t) const noexcept;

        float closest (point2f) const noexcept;

        sdot_t sdot (point2f) const noexcept;

        float sdistance2 (point2f) const noexcept;
        float sdistance  (point2f) const noexcept;

        float distance2 (point2f) const noexcept;
        float distance  (point2f) const noexcept;

        region2f region (void) const;

        point2f& operator[] (size_t idx);
        const point2f& operator[] (size_t idx) const;

        const point2f* begin  (void) const;
        const point2f* end    (void) const;
        const point2f* cbegin (void) const;
        const point2f* cend   (void) const;

    private:
        // HACK: allow easy access to component-wise arithmetic using
        // vector2f rather than point2f in the implementation.
        union {
            point2f  m_points[N+1];
            vector2f m_coeffs[N+1];
        };
    };

    template <size_t N>
    std::ostream& operator<< (std::ostream&, const bezier<N>&);
}

#endif
