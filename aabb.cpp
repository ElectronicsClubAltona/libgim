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
 * Copyright 2015 Danny Robson <danny@nerdcruft.net>
 */


#include "aabb.hpp"
#include "debug.hpp"

using util::AABB;

//-----------------------------------------------------------------------------
template <size_t S, typename T>
AABB<S,T>::AABB (point<S,T> _p0, point<S,T> _p1):
    p0 (_p0),
    p1 (_p1)
{
    debug::sanity (*this);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::extent<S,T>
AABB<S,T>::magnitude (void) const
{
    extent<S,T> out;
    for (size_t i = 0; i < S; ++i)
        out[i] = p1[i] - p0[i];
    return out;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
bool
AABB<S,T>::overlaps (point<S,T> p) const
{
    for (size_t i = 0; i < S; ++i)
        if (p0[i] > p[i] || p1[i] < p[i])
            return false;

    return true;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::point<S,T>
AABB<S,T>::closest (point<S,T> q) const
{
    point<S,T> res;

    for (size_t i = 0; i < S; ++i)
        res[i] = q[i] < p0[i] ? p0[i] :
                 q[i] > p1[i] ? p1[i] :
                 q[i];

    return res;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
AABB<S,T>&
AABB<S,T>::expand (util::vector<S,T> mag)
{
    p0 -= mag / T{2};
    p1 += mag / T{2};

    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
AABB<S,T>&
AABB<S,T>::expand (T t)
{
    return expand (vector<S,T> {t});
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
AABB<S,T>
AABB<S,T>::expanded (vector<S,T> mag)
{
    auto ret = *this;
    ret.expand (mag);
    return ret;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
AABB<S,T>
AABB<S,T>::expanded (T t)
{
    return expanded (vector<S,T> {t});
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
AABB<S,T>&
AABB<S,T>::contract (util::vector<S,T> mag)
{
    // Avoid contracting magnitudes larger than our extent
    auto diff = p1 - p0;
    auto delta = min (diff, mag);

    p0 += delta / T{2};
    p1 -= delta / T{2};

    return *this;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
AABB<S,T>&
AABB<S,T>::contract (T mag)
{
    return contract (util::vector<S,T> {mag});
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
AABB<S,T>
AABB<S,T>::contracted (util::vector<S,T> mag) const
{
    AABB<S,T> res = *this;
    res.contract (mag);
    return res;
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
AABB<S,T>
AABB<S,T>::contracted (T mag) const
{
    return contracted (vector<S,T> {mag});
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
void
AABB<S,T>::cover (point<S,T> p)
{
    p0 = min (p, p0);
    p1 = max (p, p1);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
AABB<S,T>
AABB<S,T>::operator+ (vector<S,T> v) const
{
    return { p0 + v, p1 + v };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
AABB<S,T>
AABB<S,T>::operator- (vector<S,T> v) const
{
    return { p0 - v, p1 - v };
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
bool
AABB<S,T>::operator== (const AABB<S,T> rhs) const
{
    return rhs.p0 == p0 && rhs.p1 == p1;
}



//-----------------------------------------------------------------------------
namespace debug {
    template <size_t S, typename T>
    struct validator<AABB,S,T> {
        static bool is_valid (const AABB<S,T> &b)
        {
            for (size_t i = 0; i < S; ++i)
                if (b.p1[i] < b.p0[i])
                    return false;

            return true;
        }
    };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
std::ostream&
util::operator<< (std::ostream &os, AABB<S,T> b)
{
    os << "AABB(" << b.p0 << ", " << b.p1 << ")";
    return os;
}


//-----------------------------------------------------------------------------
#define INSTANTIATE_S_T(S,T)                    \
namespace util { template struct AABB<S,T>; }   \
template bool debug::valid (const AABB<S,T>&);  \
template std::ostream& util::operator<< (std::ostream&, AABB<S,T>);

#define INSTANTIATE(T)  \
INSTANTIATE_S_T(2,T)    \
INSTANTIATE_S_T(3,T)

INSTANTIATE(uint32_t)
INSTANTIATE(uint64_t)
INSTANTIATE(float)
INSTANTIATE(double)
