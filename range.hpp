/*
 * This file is part of waif.
 *
 * Waif is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * Waif is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with waif.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2010 Danny Robson <danny@blubinc.net>
 */


#ifndef __UTIL_RANGE_HPP
#define __UTIL_RANGE_HPP

#include "json.hpp"


/**
 * Represents a continuous range of values. Contains convenience functions
 * and debugging checks.
 */
template <typename T>
struct range {
    T min;
    T max;

    range (const json::node &node);
    range (T _min, T _max);

    /// Check whether value falls within this range (inclusive)
    bool includes (T val) const;
    /// Check whether a range falls completely within (inclusive) this range
    bool includes (const range <T> &r) const;

    /// Return the closest number that falls within the range.
    T clamp (T val) const;

    /// Normalise a number to [0, 1] within the range. Does not check bounds.
    double normalise (T val) const;

    /// Return a pseudo-random uniformly distributed value within the range.
    /// There are no statistical randomness guarantees whatsoever. 
    T rand (void) const;

    bool operator ==(const range<T>& rhs) const;
    bool operator !=(const range<T>& rhs) const
        { return !(*this == rhs); }

    /// A range which is guaranteed to contain all elements type T
    static const range <T> UNLIMITED;
    /// A range which only contains elements between 0 and 1 inclusive
    static const range <T> UNIT;

    void sanity (void) const;
};


template <typename T>
std::ostream&
operator <<(std::ostream &os, const range<T> &rhs) {
    os << '[' << rhs.min << ", " << rhs.max << ']';
    return os;
}

#endif
