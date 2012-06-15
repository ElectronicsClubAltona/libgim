/*
 * This file is part of libgim.
 *
 * libgim is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * libgim is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libgim.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2011-2012 Danny Robson <danny@nerdcruft.net>
 */


#include "range.hpp"

#include "debug.hpp"
#include "maths.hpp"

#include <limits>
#include <cstdint>


//-----------------------------------------------------------------------------
using namespace std;
using namespace util;


//-----------------------------------------------------------------------------
template <typename T>
range<T>::range (const json::node &node) {
    if (node.is_string () && (node == "UNIT" ||
                              node == "unit")) {
        min = UNIT.min;
        max = UNIT.max;
    } else if (node.is_string () && (node == "UNLIMITED" ||
                                     node == "unlimited")) {
        min = UNLIMITED.min;
        max = UNLIMITED.max;
    } else {
        min = node[0].as_number ();
        max = node[1].as_number ();
    }

    sanity ();
}


template <typename T>
range<T>::range (T _min, T _max):
        min (_min),
        max (_max)
    { sanity (); }


template <typename T>
bool
range<T>::contains (T val) const
    { return val >= min && val <= max; }


template <typename T>
bool
range<T>::contains (const range <T> &r) const
    { return r.min >= min && r.max <= max; }


template <typename T>
void
range<T>::sanity (void) const
    { CHECK (min <= max); }


namespace util {
    template <>
    void
    range<double>::sanity (void) const {
        if (std::isnan (min) || std::isnan (max))
            return;
        CHECK (min <= max);
    }
}


template <typename T>
T
range<T>::clamp (T val) const
    { return std::max (min, std::min (val, max)); }


template <typename T>
void
range<T>::expand (T val) {
    // The arguments to min and max are such that expansion from initial NaN
    // values should change both min and max to be that value.
    min = std::min (val, min);
    max = std::max (val, max);
}


template <typename T>
double
range<T>::normalise (T val) const {
    CHECK_SOFT (val >= min && val <= max);
    return ((double)val - min) /
           ((double)max - min);
}


template <typename T>
range<T>&
range<T>::operator*= (T val) {
    min *= val;
    max *= val;

    return *this;
}


template <typename T>
range<T>
range<T>::operator* (T val) const {
    return range<T> (min * val, max * val);
}


namespace util {
    template <>
    double
    range<double>::random (void) const {
        double pos = ::rand () / (double)(RAND_MAX);
        return (max - min) * pos + min;
    } 

    template <>
    float
    range<float>::random (void) const {
        float pos = ::rand () / (float)(RAND_MAX);
        return (max - min) * pos + min;
    }
}

template <typename T>
T
range<T>::random (void) const {
    return min + (T)::rand () % (max - min);
}


namespace util {
    template <>
    bool
    range<float>::operator ==(const range<float> &rhs) const
        { return almost_equal (min, rhs.min) &&
                 almost_equal (max, rhs.max); }


    template <>
    bool
    range<double>::operator ==(const range<double> &rhs) const
        { return almost_equal (min, rhs.min) &&
                 almost_equal (max, rhs.max); }
}


template <typename T>
bool
range<T>::operator ==(const range<T> &rhs) const
    { return min == rhs.min && max == rhs.max; }


template <typename T> 
const range<T>
range<T>::UNLIMITED (numeric_limits<T>::has_infinity ? -numeric_limits<T>::infinity () :
                                                        numeric_limits<T>::lowest   (),
                     numeric_limits<T>::has_infinity ?  numeric_limits<T>::infinity () :
                                                        numeric_limits<T>::max      ());

template <typename T>
const range<T>
range<T>::MAX (numeric_limits<T>::lowest (),
               numeric_limits<T>::max ());


template <typename T>
const range<T>
range<T>::UNIT (0.0, 1.0);


namespace util {
    template struct range<double>;
    template struct range<float>;
    template struct range<uint8_t>;
    template struct range<uint16_t>;
    template struct range<uint32_t>;
    template struct range<uint64_t>;
}
