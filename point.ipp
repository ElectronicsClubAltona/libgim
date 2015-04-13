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
    //-------------------------------------------------------------------------
    template <size_t S, typename T>
    template <typename U>
    typename std::common_type<T,U>::type
    util::point<S,T>::distance (const point<S,U> &rhs) const {
        return std::sqrt (distance2 (rhs));
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T>
    template <typename U>
    typename std::common_type<T,U>::type
    util::point<S,T>::distance2 (const point<S,U> &rhs) const {
        typedef typename std::common_type<T,U>::type result_t;

        result_t sum { 0 };

        for (size_t i = 0; i < S; ++i)
            sum += pow2 (this->data[i] - rhs.data[i]);

        return sum;
    }


    //-------------------------------------------------------------------------
    template <size_t S, typename T>
    template <typename U>
    typename std::common_type<T,U>::type
    util::point<S,T>::manhattan (const point<S,U> &rhs) const {
        typedef typename std::common_type<T,U>::type result_t;

        result_t sum { 0 };

        for (size_t i = 0; i < S; ++i)
            sum += std::abs (this->data[i] - rhs.data[i]);

        return sum;
    }


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
}
