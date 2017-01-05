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
 * Copyright 2016 Danny Robson <danny@nerdcruft.net>
 */

#ifndef __UTIL_ROOTS_BISECTION_HPP
#define __UTIL_ROOTS_BISECTION_HPP

#include "../maths.hpp"

#include <cmath>

namespace util::roots {
    /// find a root of a function using the bisection method
    ///
    /// the user is responsible for ensuring there is in fact a root. there
    /// is no iteration limit currently, so poorly considered arguments could
    /// result in an infinite loop.
    ///
    /// a: lower bound
    /// b: upper bound
    /// f: real function
    /// tolerance: minimum range termination condition
    template <typename T>
    T
    bisection (T a, T b, T (*f)(T), T tolerance)
    {
        // to simplify the exit conditions we assume that a..b is an
        // increasing range.
        if (a > b)
            std::swap (a, b);

        while (b - a > tolerance) {
            // calculate the midpoint
            auto   c = (a + b) / T{2};
            auto f_c = f (c);

            // return early if we happened across the root
            if (exactly_zero (f_c))
                return c;

            // check which interval we fell into, and divide for the next
            // iteration
            auto f_a = f (a);
            if (samesign (f_a, f_c))
                a = c;
            else
                b = c;
        }

        // we didn't find it exactly, but it's somewhere in this range so take
        // a punt and return the middle of the range. not sure if this is
        // valid mathematically.
        return (a + b) / T{2};
    }
}

#endif
