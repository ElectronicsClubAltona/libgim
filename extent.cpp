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

#include "extent.hpp"

#include "debug.hpp"
#include "maths.hpp"

#include <cmath>

///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
util::extent<S,T>::extent (const T  _width, const T  _height):
        w (_width),
        h (_height)
{
    static_assert (S == 2, "extents currently only support 2 dimensions");

    CHECK_GE (w, 0);
    CHECK_GE (h, 0);
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::extent<S,T>::extent (T t):
    extent (t, t)
{ ; }


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::extent<S,T>::extent (vector<S,T> _v):
    extent (_v.x, _v.y)
{ ; }


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::extent<S,T>::extent (const util::extent<S,T> &rhs):
        w (rhs.w),
        h (rhs.h)
{ ; }


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::extent<S,T>&
util::extent<S,T>::operator= (extent<S,T> rhs)
{
    w = rhs.w;
    h = rhs.h;

    return *this;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
T
util::extent<S,T>::diameter (void) const
{
    return static_cast<T> (sqrt (w * w + h * h));
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
T
util::extent<S,T>::area (void) const
{
    return w * h;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
util::extent<S,T>
util::extent<S,T>::expanded (util::vector<S,T> mag) const
{
    return {
        w + mag.x,
        h + mag.y
    };
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
util::extent<S,T>
util::extent<S,T>::expanded (T t) const
{
    return expanded (util::vector<S,T> {t});
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
float
util::extent<S,T>::aspect (void) const
{
    return static_cast<float> (w) / static_cast<float> (h);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
bool
util::extent<S,T>::empty (void) const
{
    return almost_equal (area(), 0);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
T&
util::extent<S,T>::operator[] (size_t idx)
{
    switch (idx) {
    case 0: return w;
    case 1: return h;

    default:
        unreachable ();
    }
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const T&
util::extent<S,T>::operator[] (size_t idx) const
{
    switch (idx) {
    case 0: return w;
    case 1: return h;

    default:
        unreachable ();
    }
}


//-----------------------------------------------------------------------------
template <size_t S, typename T>
size_t
util::extent<S,T>::size (void) const
{
    return S;
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
bool
util::extent<S,T>::operator ==(const extent& rhs) const
{
    return almost_equal (w, rhs.w) &&
           almost_equal (h, rhs.h);
}


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
const util::extent<S,T> util::extent<S,T>::MIN {
    0, 0
};


//-----------------------------------------------------------------------------
template <size_t S, typename T>
const util::extent<S,T> util::extent<S,T>::MAX {
    std::numeric_limits<T>::max (),
    std::numeric_limits<T>::max ()
};


///////////////////////////////////////////////////////////////////////////////
namespace debug {
    template <size_t S, typename T>
    struct validator<util::extent,S,T> {
        static bool is_valid (const util::extent<S,T> &e)
        {
            return e.w >= 0 && e.h >= 0;
        }
    };
}

template bool debug::valid (const util::extent<2,float>&);
template bool debug::valid (const util::extent<2,double>&);
template bool debug::valid (const util::extent<2,uint16_t>&);
template bool debug::valid (const util::extent<2,uint32_t>&);
template bool debug::valid (const util::extent<2,uint64_t>&);


///////////////////////////////////////////////////////////////////////////////
template <size_t S, typename T>
std::ostream&
util::operator<< (std::ostream &os, util::extent<S,T> e)
{
    os << "[" << e.w << ", " << e.h << "]";
    return os;
}


template std::ostream& util::operator<< (std::ostream&, util::extent<2,uint16_t>);
template std::ostream& util::operator<< (std::ostream&, util::extent<2,uint32_t>);
template std::ostream& util::operator<< (std::ostream&, util::extent<2,uint64_t>);
template std::ostream& util::operator<< (std::ostream&, util::extent<2,float>);
template std::ostream& util::operator<< (std::ostream&, util::extent<2,double>);


//-----------------------------------------------------------------------------
namespace util {
    template struct extent<2,uint32_t>;
    template struct extent<2,uint64_t>;
    template struct extent<2,float>;
    template struct extent<2,double>;
}
