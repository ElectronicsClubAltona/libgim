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
void
AABB<S,T>::expand (point<S,T> p)
{
    p0 = min (p, p0);
    p1 = max (p, p1);
}


//-----------------------------------------------------------------------------
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
