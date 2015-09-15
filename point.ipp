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
 * Copyright 2014-2015 Danny Robson <danny@nerdcruft.net>
 */

#include "maths.hpp"

#include <algorithm>

namespace util {
    ///------------------------------------------------------------------------
    /// expand point to use homogenous coordinates of a higher dimension.
    /// ie, fill with (0,..,0,1)
    template <size_t S, typename T>
    template <size_t D>
    point<D,T>
    point<S,T>::homog (void) const
    {
        static_assert (D > S, "homog will not overwrite data");

        point<D,T> out;

        // Copy the existing data
        auto c = std::copy (this->begin (),
                            this->end (),
                            out.begin ());

        // Fill until the second last element with zeros
        auto f = std::fill_n (c, D - S - 1, T{0});

        // Last element should be one
        *f = T{1};

        return out;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, typename U>
    constexpr typename std::common_type<T,U>::type
    distance (point<S,T> a, point<S,U> b)
    {
        using type_t = typename std::common_type<T,U>::type;
        static_assert (std::is_floating_point<type_t>::value,
                       "sqrt likely requires fractional types");

        return std::sqrt (distance2 (a, b));
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, typename U>
    constexpr typename std::common_type<T,U>::type
    distance2 (point<S,T> a, point<S,U> b)
    {
        typename std::common_type<T,U>::type sum {0};

        for (size_t i = 0; i < S; ++i)
            sum += pow2 (a.data[i] - b.data[i]);

        return sum;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, typename U>
    constexpr typename std::common_type<T,U>::type
    manhattan (point<S,T> a, point<S,U> b)
    {
        typename std::common_type<T,U>::type sum {0};

        for (size_t i = 0; i < S; ++i)
            sum += util::abs (a.data[i] - b.data[i]);

        return sum;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T, typename U>
    constexpr typename std::common_type<T,U>::type
    chebyshev(point<S,T> a, point<S,U> b)
    {
        return util::max (abs (a - b));
    }
}
