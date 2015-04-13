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


#ifndef __UTIL_RANGE_HPP
#define __UTIL_RANGE_HPP

#include <cstdint>
#include <iostream>

namespace util {
    /**
     * Represents a continuous range of values. Contains convenience functions
     * and debugging checks.
     */
    template <typename T>
    struct range {
        T min;
        T max;

        range (T _min, T _max);

        T magnitude (void) const;

        /// Check whether value falls within this range (inclusive)
        bool contains (T val) const;
        /// Check whether a range falls completely within (inclusive) this range
        bool contains (const range <T> &r) const;
        /// Check whether a range falls partially within (inclusive) this range
        //bool includes (const range <T> &r) const;

        /// interpolate between min-max using the unit position
        T at (float) const;

        /// Return the closest number that falls within the range.
        T clamp (T val) const;

        /// Expand the range to include this value if necessary
        void expand (T val);

        /// Normalise a number to [0, 1] within the range. Does not check
        /// bounds, it is the caller's responsibility to clamp the result if
        /// needed.
        template <typename U>
        U normalise (T val) const;

        range& operator*= (T);
        range  operator*  (T) const;

        range& operator/= (T);
        range  operator/  (T) const;

        range& operator+= (T);
        range  operator+  (T) const;

        range& operator-= (T);
        range  operator-  (T) const;

        /// Return a pseudo-random uniformly distributed value within the range.
        /// There are no statistical randomness guarantees whatsoever.
        T random (void) const;

        bool operator ==(const range<T>& rhs) const;
        bool operator !=(const range<T>& rhs) const
            { return !(*this == rhs); }

        /// A range which is guaranteed to contain all elements type T
        static const range<T> UNLIMITED;
        static const range<T> MAX;
        /// A range which only contains elements between 0 and 1 inclusive
        static const range<T> UNIT;

        void sanity (void) const;
    };


    // convenience typedefs
    typedef util::range<float> rangef;
    typedef util::range<uintmax_t> rangeu;
    typedef util::range<intmax_t> rangei;

    // ostream operators
    template <typename T>
    std::ostream&
    operator <<(std::ostream &os, const range<T> &rhs) {
        os << '[' << rhs.min << ", " << rhs.max << ']';
        return os;
    }
}

#include "range.ipp"

#endif
