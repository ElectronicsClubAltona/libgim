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
 * Copyright 2010-2015 Danny Robson <danny@nerdcruft.net>
 */


#include "region.hpp"

#include "debug.hpp"
#include "types/casts.hpp"

#include <cmath>
#include <type_traits>


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>::region (extent_t _extent):
    region (point_t::ORIGIN, _extent)
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
    region (_a, _b - _a)
{
    debug::sanity (*this);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>::region (std::array<T,S*2> args)
{
    std::copy (&args[0], &args[S],   p.data);
    std::copy (&args[S], &args[S*2], e.data);
}

//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>::region (position_type _p,
                           size_type     _e):
    region (point_t {_p}, extent_t {_e})
{
    debug::sanity (*this);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::size_type
util::region<S,T>::area (void) const
{
    return e.area ();
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::size_type
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
void
util::region<S,T>::scale (T factor)
{
    auto o = (e * factor - e) / T{2};
    p -= o;
    e *= factor;
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
template <size_t S, typename T>
bool
util::region<S,T>::includes (point_t q) const
{
    for (size_t i = 0; i < S; ++i)
        if (q[i] < p[i] || q[i] > p[i] + e[i])
            return false;

    return true;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
bool
util::region<S,T>::contains (point_t q) const
{
    for (size_t i = 0; i < S; ++i)
        if (q[i] <= p[i] || q[i] >= p[i] + e[i])
            return false;

    return true;
}


//-----------------------------------------------------------------------------
// FIXME: This will fail with an actual infinite range (NaNs will be generated
// in the conditionals).
template <size_t S, typename T>
bool
util::region<S,T>::intersects (region<S,T> rhs) const
{
    for (size_t i = 0; i < S; ++i)
        if (p[i]     >= rhs.p[i] + rhs.e[i] ||
            rhs.p[i] >=     p[i] +     e[i])
        { return false; }

    return true;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
void
util::region<S,T>::constrain (point_t &q) const
{
    for (size_t i = 0; i < S; ++i)
        q[i] = limit (q[i], p[i], p[i] + e[i]);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
typename util::region<S,T>::point_t
util::region<S,T>::constrained (point_t q) const
{
    constrain (q);
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
        a[i] = max (p[i],        rhs.p[i]);
        b[i] = min (p[i] + e[i], rhs.p[i] + rhs.e[i]);

        if (b[i] < a[i])
            throw std::logic_error ("no overlap");
    }

    return { a, b };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>&
util::region<S,T>::resize (extent<S,T> _e)
{
    e = _e;
    return *this;
}

//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>
util::region<S,T>::inset (T mag)
{
    // ensure we have enough space to inset
    CHECK (min (e) >= 2 * mag);

    return {
        p + mag,
        e - 2 * mag
    };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>&
util::region<S,T>::expand (vector<S,T> v)
{
    p -= v;
    e += v * T{2};

    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>&
util::region<S,T>::expand (T mag)
{
    return expand ({mag});
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>
util::region<S,T>::expanded (vector<S,T> v) const
{
    return {
        p - v,
        e + v * T{2}
    };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::region<S,T>
util::region<S,T>::expanded (T mag) const
{
    return expanded ({mag});
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


///----------------------------------------------------------------------------
/// The largest specifiable finite region.
///
/// Starts at half the minimum value to allow the width to cover some positive
/// range rather than just cancelling out the lowest value for signed types.
///
/// Specifically does not allow infinities. Use/define INFINITE when required.

template <size_t S, typename T>
const util::region<S,T>
util::region<S,T>::MAX (
    std::numeric_limits<T>::lowest () / 2,
    std::numeric_limits<T>::max ()
);


template  <size_t S, typename T>
const util::region<S,T>
util::region<S,T>::UNIT ({0}, {1});


//-----------------------------------------------------------------------------
template <size_t S, typename T>
std::ostream&
util::operator<< (std::ostream &os, const util::region<S,T> &rhs) {
    os << "region(" << rhs.p << ", " << rhs.e << ")";
    return os;
}


///////////////////////////////////////////////////////////////////////////////
namespace debug {
    template <size_t S, typename T>
    struct validator<util::region,S,T> {
        static bool is_valid (const util::region<S,T> &r)
        {
            CHECK_GE (r.area (), 0);
            CHECK_GE (min (r.e), 0);

            return r.area () >= 0 && min (r.e) >= 0;
        }
    };
}


///////////////////////////////////////////////////////////////////////////////
#define INSTANTIATE_S_T(S,T)                                                \
namespace util {                                                            \
    template struct region<S,T>;                                            \
    template std::ostream& operator<< (std::ostream&, const region<S,T>&);  \
}                                                                           \
namespace debug { template struct debug::validator<util::region,S,T>; }

#define INSTANTIATE(T)  \
INSTANTIATE_S_T(2,T)    \
INSTANTIATE_S_T(3,T)

INSTANTIATE(uint32_t)
INSTANTIATE(uint64_t)
INSTANTIATE(float)
INSTANTIATE(double)
