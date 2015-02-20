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
template <typename T>
util::region<T>::region (point<2,T> _point,
                         extent<size_type> _size):
    x (_point.x),
    y (_point.y),
    w (_size.w),
    h (_size.h)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
util::region<T>::region (point<2,T> _a,
                         point<2,T> _b):
    x (_a.x),
    y (_a.y),
    w (_b.x - _a.x),
    h (_b.y - _a.y)
{
    CHECK_GE (_b.x, _a.x);
    CHECK_GE (_b.y, _a.y);
}

//-----------------------------------------------------------------------------
template <typename T>
util::region<T>::region (position_type _x,
                         position_type _y,
                         size_type _w,
                         size_type _h):
    x (_x),
    y (_y),
    w (_w),
    h (_h)
{ ; }


//-----------------------------------------------------------------------------
template <typename T>
typename util::region<T>::size_type
util::region<T>::area (void) const
{
    return w * h;
}


//-----------------------------------------------------------------------------
template <typename T>
typename util::region<T>::size_type
util::region<T>::diameter (void) const
{
    return static_cast<size_type> (std::sqrt (w * w + h * h));
}


//-----------------------------------------------------------------------------
template <typename T>
util::extent<typename util::region<T>::size_type>
util::region<T>::size (void) const
{
    return { w, h };
}


//-----------------------------------------------------------------------------
template <typename T>
void
util::region<T>::scale (T factor)
{
    x -= (w * factor - w) / T{2};
    y -= (h * factor - h) / T{2};

    w = w * factor;
    h = h * factor;
}


//-----------------------------------------------------------------------------
template <typename T>
bool
util::region<T>::empty (void) const
{
    return almost_zero (area ());
}


//-----------------------------------------------------------------------------
template <typename T>
util::point<2,T>
util::region<T>::base (void) const
{
    return { x, y };
}


//-----------------------------------------------------------------------------
template <typename T>
util::point<2,T>
util::region<T>::away (void) const
{
    return { x + w, y + h };
}


//-----------------------------------------------------------------------------
template <typename T>
util::point<2,T>
util::region<T>::centre (void) const
{
    T cx = x + w / T{2},
      cy = y + h / T{2};

    return point<2,T> { cx, cy };
}


//-----------------------------------------------------------------------------
template <typename T>
util::point<2,T>
util::region<T>::closest (point<2,T> p) const
{
    return {
        p.x < x     ? x     :
        p.x > x + w ? x + w :
        p.x,

        p.y < y     ? y     :
        p.y > y + h ? y + h :
        p.y
    };
}


//-----------------------------------------------------------------------------
template <typename T>
bool
util::region<T>::includes (const point<2,T> &p) const
{
    return p.x >= x &&
           p.y >= y &&
           p.x - x <= w &&
           p.y - y <= h;
}


//-----------------------------------------------------------------------------
template <typename T>
bool
util::region<T>::contains (const point<2,T> &p) const
{
    return p.x > x &&
           p.y > y &&
           p.x - x < w &&
           p.y - y < h;
}


//-----------------------------------------------------------------------------
// FIXME: This will fail with an actual infinite range (NaNs will be generated
// in the conditionals).
template <typename T>
bool
util::region<T>::intersects (const util::region<T> &rhs) const
{
    return  x     < rhs.x + rhs.w &&
            rhs.x < x     +     w &&
            y     < rhs.y + rhs.h &&
            rhs.y < y     +     h;
}


//-----------------------------------------------------------------------------
template <typename T>
void
util::region<T>::constrain (point<2,T> &p) const
{
    p.x = std::min (std::max (p.x, x), x + w);
    p.y = std::min (std::max (p.y, y), y + h);
}


//-----------------------------------------------------------------------------
template <typename T>
util::point<2,T>
util::region<T>::constrained (const point<2,T> &p) const
{
    point<2,T> v;
    v.x = std::min (std::max (p.x, x), x + w);
    v.y = std::min (std::max (p.y, y), y + h);

    return v;
}


//-----------------------------------------------------------------------------
template<typename T>
util::region<T>
util::region<T>::intersection (const util::region<T> &rhs) const
{
    T newx1 = max (x, rhs.x),
      newy1 = max (y, rhs.y),
      newx2 = min (x + sign_cast<T> (w), rhs.x + sign_cast<T> (rhs.w)),
      newy2 = min (y + sign_cast<T> (h), rhs.y + sign_cast<T> (rhs.h));

    if (newx2 < newx1 || newy2 < newy1)
        throw std::logic_error ("No overlap");

    size_type nw = sign_cast<size_type> (newx2 - newx1);
    size_type nh = sign_cast<size_type> (newy2 - newy1);
    return util::region<T> (newx1, newy1, nw, nh);
}


//-----------------------------------------------------------------------------
template <typename T>
util::region<T>
util::region<T>::inset (T mag)
{
    CHECK_GE (w - x, 2 * mag);
    CHECK_GE (h - y, 2 * mag);

    return { x + mag, y + mag, w - 2 * mag, h - 2 * mag };
}


//-----------------------------------------------------------------------------
template <typename T>
util::region<T>&
util::region<T>::expand (T _w, T _h)
{
    x -= _w;
    y -= _h;
    w += _w * 2;
    h += _h * 2;

    return *this;
}


//-----------------------------------------------------------------------------
template <typename T>
util::region<T>&
util::region<T>::expand (T mag)
{
    return expand (mag, mag);
}


//-----------------------------------------------------------------------------
template <typename T>
util::region<T>
util::region<T>::expanded (T _w, T _h) const
{
    return {
        x - _w,
        y - _h,
        w + _w * 2,
        h + _h * 2,
    };
}


//-----------------------------------------------------------------------------
template <typename T>
util::region<T>
util::region<T>::expanded (T mag) const
{
    return expanded (mag, mag);
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
util::region<T>
util::region<T>::operator+ (vector<2,T> rhs) const
{
    return { x + rhs.x, y + rhs.y, w, h };
}


///////////////////////////////////////////////////////////////////////////////
template <typename T>
bool
util::region<T>::operator== (const region& rhs) const
{
    return almost_equal (x, rhs.x) &&
           almost_equal (y, rhs.y) &&
           almost_equal (w, rhs.w) &&
           almost_equal (h, rhs.h);
}


//-----------------------------------------------------------------------------
template <typename T>
void
util::region<T>::sanity (void) const {
    CHECK_GE (w, 0);
    CHECK_GE (h, 0);
    static_assert(!std::is_floating_point<T>::value,
                  "Floating point types need width and height checks");
}


//-----------------------------------------------------------------------------
namespace util {
    template <>
    void region<double>::sanity (void) const {
        CHECK_GE (w, 0);
        CHECK_GE (h, 0);
    }


    template <>
    void region<float>::sanity (void) const {
        CHECK_GE (w, 0);
        CHECK_GE (h, 0);
    }
}


///----------------------------------------------------------------------------
/// The largest specifiable finite region.
///
/// Starts at half the minimum value to allow the width to cover some positive
/// range rather than just cancelling out the lowest value for signed types.
///
/// Specifically does not allow infinities. Use/define INFINITE when required.

template <typename T>
const util::region<T>
util::region<T>::MAX (
    std::numeric_limits<T>::lowest () / 2,
    std::numeric_limits<T>::lowest () / 2,
    std::numeric_limits<T>::max (),
    std::numeric_limits<T>::max ()
);


template  <typename T>
const util::region<T>
util::region<T>::UNIT (0, 0, 1, 1);


//-----------------------------------------------------------------------------
template <typename T>
std::ostream&
util::operator<< (std::ostream &os, const util::region<T> &rhs) {
    os << "region(" << rhs.x << ", " << rhs.y << ", " << rhs.w << ", " << rhs.h << ")";
    return os;
}


//-----------------------------------------------------------------------------
namespace util {
    template struct region<uint32_t>;
    template struct region<uint64_t>;
    template struct region<float>;
    template struct region<double>;

    template std::ostream& operator<< (std::ostream&, const region< int32_t>&);
    template std::ostream& operator<< (std::ostream&, const region< int64_t>&);
    template std::ostream& operator<< (std::ostream&, const region<uint32_t>&);
    template std::ostream& operator<< (std::ostream&, const region<uint64_t>&);
    template std::ostream& operator<< (std::ostream&, const region<   float>&);
    template std::ostream& operator<< (std::ostream&, const region<  double>&);
}
