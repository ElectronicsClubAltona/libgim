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
 * Copyright 2010-2017 Danny Robson <danny@nerdcruft.net>
 */


#include "./region.hpp"

#include "./debug.hpp"

#include "./coord/iostream.hpp"

#include <array>

using util::region;


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>::region (extent_t _extent):
    region (point_t::origin (), _extent)
{
    debug::sanity (*this);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>::region (point_t  _p,
                           extent_t _e):
    p (_p),
    e (_e)
{
    debug::sanity (*this);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>::region (point_t _a,
                           point_t _b):
    region (_a, extent_t { _b - _a })
{
    // This check must allow for zero area (but non-zero dimension) regions.
    // Some code paths need to support this degenerate case. It's ugly but
    // simplifies generalisation. eg, vertical linear bezier curves.
    CHECK (all (_a <= _b));

    debug::sanity (*this);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
T
util::region<S,T>::area (void) const
{
    return e.area ();
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
T
util::region<S,T>::diameter (void) const
{
    return e.diameter ();
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::extent_t
util::region<S,T>::magnitude (void) const
{
    return e;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::extent_t
util::region<S,T>::magnitude (extent_t _e)
{
    e = _e;
    return e;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
bool
util::region<S,T>::empty (void) const
{
    return almost_zero (area ());
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::point_t
util::region<S,T>::base (void) const
{
    return p;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::point_t
util::region<S,T>::away (void) const
{
    return p + e;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::point_t
util::region<S,T>::centre (void) const
{
    return p + e / T{2};
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::point_t
util::region<S,T>::closest (point_t q) const
{
    point_t out;

    for (size_t i = 0; i < S; ++i)
        out[i] = q[i] < p[i] ? p[i]        :
                 q[i] > p[i] ? p[i] + e[i] :
                 q[i];

    return out;
}


//-----------------------------------------------------------------------------
// FIXME: This will fail with an actual infinite range (NaNs will be generated
// in the conditionals).
template <size_t S, typename T>
bool
util::region<S,T>::intersects (region<S,T> rhs) const
{
    for (size_t i = 0; i < S; ++i)
        if (    p[i] >= rhs.p[i] + rhs.e[i] ||
            rhs.p[i] >=     p[i] +     e[i])
        { return false; }

    return true;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename region<S,T>::point_t
region<S,T>::constrain (point_t q) const noexcept
{
    for (size_t i = 0; i < S; ++i)
        q[i] = limit (q[i], p[i], p[i] + e[i]);
    return q;
}


//-----------------------------------------------------------------------------
template<size_t S, typename T>
util::region<S,T>
util::region<S,T>::intersection (region<S,T> rhs) const
{
    // find the intersection corners
    point_t a, b;

    for (size_t i = 0; i < S; ++i) {
        a[i] = util::max (p[i],        rhs.p[i]);
        b[i] = util::min (p[i] + e[i], rhs.p[i] + rhs.e[i]);

        if (b[i] < a[i])
            throw std::logic_error ("no overlap");
    }

    return { a, b };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
bool
util::region<S,T>::covers (region<S, T> r) const noexcept
{
    return all (p <= r.p) && all (p + e >= r.p + r.e);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
util::region<S,T>
util::region<S,T>::inset (T mag) const
{
    return inset (util::vector<S,T> {mag});
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>
util::region<S,T>::inset (vector<S,T> mag) const
{
    // ensure we have enough space to trim off our total extent
    CHECK (all (e >= 2 * mag));

    return {
        p +        mag,
        e - T{2} * mag
    };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>
util::region<S,T>::expand (vector<S,T> v) const
{
    return {
        p - v,
        e + v * T{2}
    };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>
util::region<S,T>::expand (T mag) const
{
    return expand (vector<S,T> {mag});
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
util::region<S,T>
util::region<S,T>::operator+ (vector<S,T> rhs) const
{
    return { p + rhs, e };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>
util::region<S,T>::operator- (vector<S,T> rhs) const
{
    return { p - rhs, e };
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
bool
util::region<S,T>::operator== (region rhs) const
{
    return p == rhs.p && e == rhs.e;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
std::ostream&
util::operator<< (std::ostream &os, const util::region<S,T> &rhs) {
    return os << "{ position: " << rhs.p << ", extent: " << rhs.e << " }";
}


///////////////////////////////////////////////////////////////////////////////
namespace util::debug {
    template <size_t S, typename T>
    struct validator<util::region<S,T>> {
        static bool is_valid (const util::region<S,T> &r)
        {
            return util::debug::is_valid (r.p) && util::debug::is_valid (r.e);
        }
    };
}


///////////////////////////////////////////////////////////////////////////////
#define INSTANTIATE_S_T(S,T)                                                 \
template struct util::region<S,T>;                                           \
template std::ostream& util::operator<< (std::ostream&, const region<S,T>&); \
template struct util::debug::validator<util::region<S,T>>;

#define INSTANTIATE(T)  \
INSTANTIATE_S_T(2,T)    \
INSTANTIATE_S_T(3,T)

INSTANTIATE(int16_t);
INSTANTIATE(int32_t);
INSTANTIATE(int64_t);

INSTANTIATE(uint16_t)
INSTANTIATE(uint32_t)
INSTANTIATE(uint64_t)

INSTANTIATE(float)
INSTANTIATE(double)
