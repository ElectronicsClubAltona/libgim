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
 * Copyright 2011-2012 Danny Robson <danny@nerdcruft.net>
 */


#include "range.hpp"

#include "debug.hpp"
#include "json/tree.hpp"
#include "maths.hpp"

#include <cmath>
#include <cstdint>


///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::range<T>::range (T _lo, T _hi):
        lo (_lo),
        hi (_hi)
{
    sanity ();
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
T
util::range<T>::magnitude (void) const
{
    return hi - lo;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool
util::range<T>::contains (T val) const
{
    return val >= lo && val <= hi;
}


//-----------------------------------------------------------------------------
template <typename T>
bool
util::range<T>::contains (const range <T> &r) const
{
    return r.lo >= lo && r.hi <= hi;
}


//-----------------------------------------------------------------------------
template <typename T>
T
util::range<T>::at (float t) const
{
    return static_cast<T> (lo + (hi - lo) * t);
}


//-----------------------------------------------------------------------------
template <typename T>
T
util::range<T>::clamp (T val) const
{
    return std::max (lo, std::min (val, hi));
}


//-----------------------------------------------------------------------------
template <typename T>
void
util::range<T>::expand (T val)
{
    // The arguments to min and max are such that expansion from initial NaN
    // values should change both min and max to be that value.
    lo = std::min (val, lo);
    hi = std::max (val, hi);
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::range<T>&
util::range<T>::operator*= (T val)
{
    lo *= val;
    hi *= val;

    return *this;
}


//-----------------------------------------------------------------------------
template <typename T>
util::range<T>
util::range<T>::operator* (T val) const
{
    return util::range<T> (lo * val, hi * val);
}


//-----------------------------------------------------------------------------
template <typename T>
util::range<T>
util::range<T>::operator- (T val) const
{
    return util::range<T> (lo - val, hi - val);
}


///////////////////////////////////////////////////////////////////////////////
namespace util {
    template <>
    double
    range<double>::random (void) const
    {
        double pos = ::rand () / (double)(RAND_MAX);
        return (hi - lo) * pos + lo;
    }

    template <>
    float
    range<float>::random (void) const
    {
        float pos = ::rand () / (float)(RAND_MAX);
        return (hi - lo) * pos + lo;
    }
}


//-----------------------------------------------------------------------------
template <typename T>
T
util::range<T>::random (void) const
{
    return lo + (T)::rand () % (hi - lo);
}


///////////////////////////////////////////////////////////////////////////////
namespace util {
    template <>
    bool
    range<float>::operator ==(const range<float> &rhs) const
    {
        return almost_equal (lo, rhs.lo) &&
               almost_equal (hi, rhs.hi);
    }


    template <>
    bool
    range<double>::operator ==(const range<double> &rhs) const
    {
        return almost_equal (lo, rhs.lo) &&
               almost_equal (hi, rhs.hi);
    }
}


//-----------------------------------------------------------------------------
template <typename T>
bool
util::range<T>::operator ==(const util::range<T> &rhs) const
{
    return lo == rhs.lo && hi == rhs.hi;
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
void
util::range<T>::sanity (void) const
{
    CHECK (lo <= hi);
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    void
    range<double>::sanity (void) const
    {
        if (std::isnan (lo) || std::isnan (hi))
            return;
        CHECK (lo <= hi);
    }
}


//-----------------------------------------------------------------------------
template struct util::range<double>;
template struct util::range<float>;
template struct util::range<int8_t>;
template struct util::range<int16_t>;
template struct util::range<int32_t>;
template struct util::range<int64_t>;
template struct util::range<uint8_t>;
template struct util::range<uint16_t>;
template struct util::range<uint32_t>;
template struct util::range<uint64_t>;


//-----------------------------------------------------------------------------
namespace json::tree {
    template <>
    util::range<double>
    io<util::range<double>>::deserialise (const json::tree::node &node)
    {
        if (node.is_string () && (node == "UNIT" ||
                                  node == "unit")) {
            return util::range<double>::unit ();
        } else if (node.is_string () && (node == "UNLIMITED" ||
                                         node == "unlimited")) {
            return util::range<double>::unlimited ();
        } else {
            return {
                node[0].as_number (),
                node[1].as_number ()
            };
        }
    }
}
